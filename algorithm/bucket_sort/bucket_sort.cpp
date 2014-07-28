//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Bucket Sort
//    Divide data into multiple buckets, and combine these buckets together.
//
//    This algorithm has an O(n) time complexity, the drawback is that it also has an O(n) spece complexity.
//    Also, because of issues like cache-missing, etc, it has advantage over quick-sort/mrege-sort etc. only
//    when data volume is quite big. Another drawback is that, it only works with integral data typess.
//
//    This is only a very simple implementation, only to demonstrate basic algorithm concepts
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iterator>
#include <algorithm>
#include <array>
#include <vector>
#include <utility>
#include <random>
#include <chrono>

using namespace std::chrono;

constexpr int DEFAULT_BUCKET_NUMBER = 16;

//template<int N_BUCKETS>
template<class BiIter>
void bucket_sort(BiIter data_begin, BiIter data_end)
{
	int length = std::distance(data_begin, data_end);
	if (length <= 1)
		return;

	// Generally, it is hard to define bucket number or bucket size for all situations, so it's also hard to reserve vector's size
	
	std::pair<BiIter, BiIter> min_max = std::minmax_element(data_begin, data_end);
	auto data_min = *(min_max.first);
	auto data_max = *(min_max.second);
	int data_range = data_max - data_min;

	int num_buckets = DEFAULT_BUCKET_NUMBER;
	int bucket_size = data_range / num_buckets;
	if(bucket_size == 0)
		bucket_size = 1;
	++num_buckets;
	
	typedef decltype(*data_begin + *data_end) T;
	std::vector<std::vector<T>> buckets(num_buckets+1);
	
	BiIter itr = data_begin;
	while(itr != data_end)
	{
		auto x = *itr++;
		buckets[(x - data_min) / bucket_size].push_back(x);
	}
	for (unsigned int i=0; i<buckets.size(); ++i)
		std::sort(buckets[i].begin(), buckets[i].end());

	itr = data_begin;
	for (unsigned int i = 0; i<buckets.size(); ++i)
	{
		for (unsigned int j = 0; j<buckets[i].size(); ++j)
			*itr++ = buckets[i][j];
	}
}

constexpr int N = 37;
int main()
{
	std::array<int, N> arr;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distr1(-100, 100);

	for(int i=0; i<N; ++i)
		arr[i] = distr1(generator);
		
	std::cout << "Before bucket sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << arr[i] << ", ";
	std::cout << std::endl;
	
	bucket_sort(arr.begin(), arr.end());

	std::cout << "After bucket sort: " << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << arr[i] << ", ";
	std::cout << std::endl << std::endl;

	std::cout << "*******************************************************" << std::endl;
	std::cout << "*** bucket_sort vs std::sort on 1 millioin integers ***" << std::endl;
	std::cout << "*******************************************************" << std::endl;

	std::array<int, 1000000> big_arr;
	std::uniform_int_distribution<int> distr2(-500000, 500000);
	
	for(int i=0; i<1000000; ++i)
		big_arr[i] = distr2(generator);
	auto t0 = high_resolution_clock::now();
	bucket_sort(big_arr.begin(), big_arr.end());
	auto t1 = high_resolution_clock::now();

	for(int i=0; i<1000000; ++i)
		big_arr[i] = distr2(generator);
	auto t2 = high_resolution_clock::now();
	std::sort(big_arr.begin(), big_arr.end());
	auto t3 = high_resolution_clock::now();

	std::cout << "Times of bucket_sort and std::sort are: " << duration_cast<milliseconds>(t1-t0).count() << " milliseconds, " << duration_cast<milliseconds>(t3-t2).count() << " milliseconds." << std::endl << std::endl;

	std::cout << "*******************************************************" << std::endl;
	std::cout << "*** bucket_sort vs std::sort on 10 million integers ***" << std::endl;
	std::cout << "*******************************************************" << std::endl;

	std::vector<int> big_arr2;
	big_arr2.resize(10000000);
	std::uniform_int_distribution<int> distr3(-5000000, 5000000);
	
	for(int i=0; i<10000000; ++i)
		big_arr2[i] = distr3(generator);
	auto t4 = high_resolution_clock::now();
	bucket_sort(big_arr2.begin(), big_arr2.end());
	auto t5 = high_resolution_clock::now();

	for(int i=0; i<10000000; ++i)
		big_arr2[i] = distr3(generator);
	auto t6 = high_resolution_clock::now();
	std::sort(big_arr2.begin(), big_arr2.end());
	auto t7 = high_resolution_clock::now();

	std::cout << "Times of bucket_sort and std::sort are: " << duration_cast<milliseconds>(t5-t4).count() << " milliseconds, " << duration_cast<milliseconds>(t7-t6).count() << " milliseconds." << std::endl;
	
	return 0;
}

