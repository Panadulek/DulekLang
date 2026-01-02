#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>
#include <utility>

struct TypeInfo {
	size_t size;
	uint32_t numPointers;
	uint32_t* pointerOffsets;
};

struct ObjHeader {
	TypeInfo* type;
	uintptr_t forwardingAddr;
};

struct OsMemory
{
	static void* allocated(size_t size);
	static void deallocated(void* ptr, size_t size);
};

class GCArena
{
	uint8_t* m_start;
	uint8_t* m_top;
	uint8_t* m_limit;
	std::size_t m_size;
public:
	GCArena(std::size_t arenaSize);
	~GCArena();

	void* allocate(std::size_t bytes);
	void reset();
	bool contains(void* ptr) const;
	uint8_t* getStart() const;
	uint8_t* getTop() const;
};

class GCHeap
{
	using GCArenaPtr = std::unique_ptr<GCArena>;
	using ArenaPair = std::pair<GCArenaPtr, GCArenaPtr>;
	ArenaPair m_spaces;
	std::vector<void**> m_roots; 
public:
	GCHeap(std::size_t arenaSize);

	void* allocate(std::size_t bytes, TypeInfo* type);
	void collect();
	void* copyObject(void* oldObj);
	void registerRoot(void** rootPtr) {
		m_roots.push_back(rootPtr);
	}
};