#include "../Du/allocators/allocator.hpp"
#include <gtest/gtest.h>
#include <inttypes.h>
#include <vector>

struct AllocatorTests
{
	static bool compare_stl_vs_slab_allocator_in_vector()
	{
		return get_slab_allocator_time() < get_stl_allocator_time();
	}
private:
	static std::size_t get_slab_allocator_time()
	{
		constexpr size_t _size = 10000;
		std::vector<int, SlabAllocator<int, _size>> vec;
		std::size_t begin =__rdtsc();
		for (std::size_t i = 0; i < _size; i++)
		{
			vec.emplace_back(i);
		}
		std::size_t end = __rdtsc();
		return end - begin;
	}
	static std::size_t get_stl_allocator_time()
	{
		constexpr size_t _size = 10000;
		std::vector<int> vec;
		std::size_t begin = __rdtsc();
		for (std::size_t i = 0; i < _size; i++)
		{
			vec.emplace_back(i);
		}
		std::size_t end = __rdtsc();
		return end - begin;
	}
};

TEST(ALLOCATOR, SLAB_VS_STL)
{
	EXPECT_TRUE(AllocatorTests::compare_stl_vs_slab_allocator_in_vector());
}