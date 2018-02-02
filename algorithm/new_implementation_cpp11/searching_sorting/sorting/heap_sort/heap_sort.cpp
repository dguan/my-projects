#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <array>

using namespace std::chrono;

////////////////////////////////////////////////////////////////////////////////////
//
//    Heap Sort
//    Heap sort is cool, it is fast and can be in-place, the drawbacks are that
//    it is slower than quick sort and it is not stable.
//    This is the in-place implementation heap-sort.
//
////////////////////////////////////////////////////////////////////////////////////

// build the heap, the heap's length = pos+1
template<class RandIter>
void adjust_heap_down(RandIter heap_begin, RandIter cur_pos, bool top_type)
{
	int length = std::distance(heap_begin, cur_pos) + 1;
	int parent = length/2 - 1;
	while (parent >= 0)
	{
		int biggest = parent;
		int lchild = biggest * 2 + 1;
		int rchild = lchild + 1;

		if (lchild<length && (top_type ? *(heap_begin + lchild) < *(heap_begin + biggest) : *(heap_begin + lchild) > *(heap_begin + biggest)))
			biggest = lchild;
		if (rchild<length && (top_type ? *(heap_begin + rchild) < *(heap_begin + biggest) : *(heap_begin + rchild) > *(heap_begin + biggest)))
			biggest = rchild;

		if (biggest != parent)
		{
			std::iter_swap(heap_begin + biggest, heap_begin + parent);
			parent = (parent - 1) / 2;
		}
		else
			break;
	}
}

// rebuild the heap from heap[0] after sorting out an item
template<class RandIter>
void adjust_heap_up(RandIter heap_begin, RandIter heap_end, bool top_type)
{
	int pos = 0;
	int length = std::distance(heap_begin, heap_end);
	while (pos < length)
	{
		int lchild = pos * 2 + 1;
		int rchild = lchild + 1;
		int biggest = pos;

		if (lchild<length && (top_type ? *(heap_begin + lchild) < *(heap_begin + biggest) : *(heap_begin + lchild) > *(heap_begin + biggest)))
			biggest = lchild;
		if (rchild<length && (top_type ? *(heap_begin + rchild) < *(heap_begin + biggest) : *(heap_begin + rchild) > *(heap_begin + biggest)))
			biggest = rchild;

		if (biggest != pos)
		{
			std::iter_swap(heap_begin + biggest, heap_begin + pos);
			pos = biggest;
		}
		else
			break;
	}
}

//This is the interface function
//top_type==false for BigTop heap, top_type==true for SmallTop heap
template<class RandIter>
void heap_sort(RandIter data_begin, RandIter data_end, bool top_type=false)
{
	if(std::distance(data_begin, data_end) <= 1)
		return;

	for (RandIter itr = data_begin+2; itr != data_end; ++itr)
		adjust_heap_down(data_begin, itr, top_type);

	for (RandIter itr = --data_end; itr != data_begin; --itr)
	{
		std::iter_swap(data_begin, itr);
		adjust_heap_up(data_begin, itr, top_type);
	}
}


// Well, std::make_heap and std::sort_heap can only make and sort a big-top heap, currently
template<class RandIter>
void heap_sort_stl(RandIter data_begin, RandIter data_end)
{
	if (std::distance(data_begin, data_end) <= 1)
		return;
	std::make_heap(data_begin, data_end);
	std::sort_heap(data_begin, data_end);
}


#define N 31
int main()
{
	std::array<int, N> raw_data;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-100,100);
	
	std::cout << "************************************" << std::endl;
	std::cout << "*** heap sort using big-top heap ***" << std::endl;
	std::cout << "************************************" << std::endl;

	for(int i=0; i<N; ++i)
		raw_data[i] = distribution(generator);
		
	std::cout << "Before heap sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ", ";
	std::cout << std::endl;
	
	heap_sort(raw_data.begin(), raw_data.end());

	std::cout << "After heap sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ", ";
	std::cout << std::endl << std::endl;

	std::cout << "**************************************" << std::endl;
	std::cout << "*** heap sort using small-top heap ***" << std::endl;
	std::cout << "**************************************" << std::endl;
	
	for(int i=0; i<N; ++i)
		raw_data[i] = distribution(generator);
		
	std::cout << "Before heap sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ", ";
	std::cout << std::endl;
	
	heap_sort(raw_data.begin(), raw_data.end(), true);

	std::cout << "After heap sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ", ";
	std::cout << std::endl << std::endl;

	std::cout << "**************************************" << std::endl;
	std::cout << "*** heap sort using STL algorithms ***" << std::endl;
	std::cout << "**************************************" << std::endl;
	
	for(int i=0; i<N; ++i)
		raw_data[i] = distribution(generator);
		
	std::cout << "Before heap sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ", ";
	std::cout << std::endl;
	
	heap_sort_stl(raw_data.begin(), raw_data.end());

	std::cout << "After heap sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ", ";
	std::cout << std::endl << std::endl;
	
	std::cout << "****************************************************" << std::endl;
	std::cout << "*** performance benchmarking with STL algorithms ***" << std::endl;
	std::cout << "****************************************************" << std::endl;

	std::array<int, 1000000> big_arr;
	std::uniform_int_distribution<int> distr2;
	
	for(int i=0; i<1000000; ++i)
		big_arr[i] = distr2(generator);
	auto t0 = high_resolution_clock::now();
	heap_sort(big_arr.begin(), big_arr.end());
	auto t1 = high_resolution_clock::now();

	for(int i=0; i<1000000; ++i)
		big_arr[i] = distr2(generator);
	auto t2 = high_resolution_clock::now();
	heap_sort_stl(big_arr.begin(), big_arr.end());
	auto t3 = high_resolution_clock::now();
	
	for(int i=0; i<1000000; ++i)
		big_arr[i] = distr2(generator);
	auto t4 = high_resolution_clock::now();
	std::sort(big_arr.begin(), big_arr.end());
	auto t5 = high_resolution_clock::now();

	std::cout << "Times of heap_sort, STL heap sort, and std::sort are: " << duration_cast<milliseconds>(t1-t0).count() << " milliseconds, " << duration_cast<milliseconds>(t3-t2).count() << " milliseconds, and " << duration_cast<milliseconds>(t5-t4).count() << " milliseconds." << std::endl;

	return 0;
}


