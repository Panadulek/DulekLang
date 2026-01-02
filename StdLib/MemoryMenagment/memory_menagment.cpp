#include <cstdlib>
#include <vector>
#include <cstring>
#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif
#include <iostream>
#include <memory>
#include "memory_menagment.h"

void* OsMemory::allocated(size_t size)
{
#if defined(_WIN32)
	void* ptr = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!ptr) {
		std::cerr << "VirtualAlloc failed with error: " << GetLastError() << std::endl;
	}
	return ptr;

#else
	void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {
		std::cerr << "mmap failed" << std::endl;
		return nullptr;
	}
	return ptr;
#endif

}

void OsMemory::deallocated(void* ptr, size_t size)
{
#if defined(_WIN32)
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, size);
#endif
}

GCArena::GCArena(std::size_t arenaSize) : m_start(static_cast<uint8_t*>(OsMemory::allocated(arenaSize))), m_size(arenaSize)
{
	m_top = m_start;
	m_limit = m_start + m_size;
}

GCArena::~GCArena()
{
	OsMemory::deallocated(m_start, m_size);
}

void* GCArena::allocate(std::size_t bytes)
{
	size_t alignedSize = (bytes + 7) & ~static_cast<size_t>(7);
	if (m_top + alignedSize > m_limit)
	{
		return nullptr; //TO DO Release GC
	}
	void* ptr = m_top;
	m_top += alignedSize;
	return ptr;
}

void GCArena::reset()
{
	m_top = m_start;
}

bool GCArena::contains(void* ptr) const
{
	return ptr >= m_start && ptr < m_limit;
}

uint8_t* GCArena::getStart() const { return m_start; }
uint8_t* GCArena::getTop() const { return m_top; }

GCHeap::GCHeap(std::size_t arenaSize) : m_spaces{ std::make_unique<GCArena>(arenaSize), std::make_unique<GCArena>(arenaSize) }
{
}

void* GCHeap::allocate(std::size_t bytes, TypeInfo* type) {
    size_t totalSize = sizeof(ObjHeader) + bytes;
    void* ptr = m_spaces.first->allocate(totalSize);
    if (!ptr) {
        collect();
        ptr = m_spaces.first->allocate(totalSize);
        if (!ptr) throw std::bad_alloc();
    }

    ObjHeader* h = static_cast<ObjHeader*>(ptr);
    h->type = type;
    h->forwardingAddr = 0;

    return static_cast<uint8_t*>(ptr) + sizeof(ObjHeader);
}

void GCHeap::collect() {
    std::swap(m_spaces.first, m_spaces.second);

    GCArena* toSpace = m_spaces.first.get();
    GCArena* fromSpace = m_spaces.second.get();

    toSpace->reset();

    uint8_t* scan = toSpace->getStart();
    for (void** rootPtr : m_roots) {
        if (*rootPtr) {
            *rootPtr = copyObject(*rootPtr);
        }
    }

    while (scan < toSpace->getTop()) {
        ObjHeader* h = reinterpret_cast<ObjHeader*>(scan);
        uint8_t* data = reinterpret_cast<uint8_t*>(h) + sizeof(ObjHeader);

        for (uint32_t i = 0; i < h->type->numPointers; ++i) {
            uint32_t offset = h->type->pointerOffsets[i];
            void** field = reinterpret_cast<void**>(data + offset);

            if (*field) {
                *field = copyObject(*field);
            }
        }

        size_t fullSize = (sizeof(ObjHeader) + h->type->size + 7) & ~7;
        scan += fullSize;
    }
    fromSpace->reset();
}

void* GCHeap::copyObject(void* oldObj) {
    GCArena* toSpace = m_spaces.first.get();
    GCArena* fromSpace = m_spaces.second.get();
    if (!oldObj) return nullptr;
    if (!fromSpace->contains(oldObj)) return oldObj;
    ObjHeader* oldHeader = reinterpret_cast<ObjHeader*>(static_cast<uint8_t*>(oldObj) - sizeof(ObjHeader));
    if (oldHeader->forwardingAddr != 0) {
        return reinterpret_cast<void*>(oldHeader->forwardingAddr);
    }
    size_t totalSize = sizeof(ObjHeader) + oldHeader->type->size;
    void* newRaw = toSpace->allocate(totalSize);
    if (!newRaw) return oldObj;
    memcpy(newRaw, oldHeader, totalSize);
    void* newObjData = static_cast<uint8_t*>(newRaw) + sizeof(ObjHeader);
    oldHeader->forwardingAddr = reinterpret_cast<uintptr_t>(newObjData);

    return newObjData;
}