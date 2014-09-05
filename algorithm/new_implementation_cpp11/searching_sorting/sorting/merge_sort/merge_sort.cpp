#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>
#include <new>
#include <memory>
#include <cstdlib>

#include <chrono>
using namespace std::chrono;

//////////////////////////////////////////////////////////////////////////
//
//    merge_sort
//
//    One of the most efficient sorting algorithm, fast and stable.
//    Only that it require an O(n) space as temporary working space,
//    if you apply in-place merge sort, then it will lose stability.
//
//    This is the most efficient non-recursive non-inplace stable merge sort.
//
/////////////////////////////////////////////////////////////////////////
template<class T>
T* merge_sort(T data[], int length)
{
	//std::unique_ptr<T[]> buff = std::make_unique<T[]>(length);	// What a pity g++ 8.4 does not support make_unique
	std::unique_ptr<T[]> buff(new(std::nothrow) T[length]);
	if (!buff)
		return nullptr;

	T *src = data;
	T *dst = buff.get();
	int low1 = 0;
	int cur_step = 1;
	int max_len = length - 1;

	while (cur_step <= max_len)
	{
		bool end_of_round = false;
		while (low1 <= max_len)
		{
			int high1 = low1 + cur_step - 1;
			if (high1 >= max_len)
				high1 = max_len;
			int low2 = high1 + 1;
			int high2 = low2 + cur_step - 1;
			if (high2 >= max_len)
			{
				high2 = max_len;
				end_of_round = true;	// We have reached the end of the data, merge the two into one
			}

			// merge the two sorted sub-data into one
			T *p = dst + low1;
			while (low1 <= high1 && low2 <= high2)
			{
				if (src[low1] <= src[low2])
					*p++ = src[low1++];
				else
					*p++ = src[low2++];
			}
			if (low1 > high1 && low2 <= high2)
			while (low2 <= high2)
				*p++ = src[low2++];
			else if (low2 > high2 && low1 <= high1)
			while (low1 <= high1)
				*p++ = src[low1++];

			if (!end_of_round)
				low1 = high2 + 1;
			else
				break;
		}
		if (cur_step >= length)	// The whole data has been sorted
			break;
		else
		{
			cur_step *= 2;
			std::swap(src, dst);	// Start another round
			low1 = 0;
		}
	}
	if (dst == data)	// Copy whole sorted data back into original array, if necessary
	for (int i = 0; i < length; ++i)
		data[i] = buff[i];

	return data;
}


//////////////////////////////////////////////////////////////////////////
//
//    merge_sort_recursive
//
//    One of the most efficient sorting algorithm, fast and stable.
//    Only that it require an O(n) space as temporary working space,
//    if you apply in-place merge sort, then it will lose stability.
//
//    This is a basic recursive none-inplace merge sort.
//
/////////////////////////////////////////////////////////////////////////

template<class T>
void merge(T data[], int length1, int length2, T *temp_buf)
{
	int low1 = 0;
	int high1 = length1 - 1;
	int low2 = high1 + 1;
	int high2 = low2 + length2 - 1;
	int temp_pos = 0;

	while ((low1 <= high1) || (low2 <= high2))
	{
		if (low1>high1)
		{
			while (low2 <= high2)
				temp_buf[temp_pos++] = data[low2++];
			break;
		}
		if (low2>high2)
		{
			while (low1 <= high1)
				temp_buf[temp_pos++] = data[low1++];
			break;
		}
		if (data[low1] <= data[low2])	//may also use std::less_equal<T>(data[low1], data[low2]) as comparetor
			temp_buf[temp_pos++] = data[low1++];	// use <= instead of < to keep stability
		else
			temp_buf[temp_pos++] = data[low2++];
	}

	for (int i = 0; i<length1 + length2; ++i)
		data[i] = temp_buf[i];
}

template<class T>
void merge_sort_recursive(T data[], int length, T *temp_buf)
{
	if (length <= 1)
		return;

	int high = length - 1;
	int mid = high / 2;
	merge_sort_recursive(data, mid + 1, temp_buf);
	merge_sort_recursive(data + mid + 1, length - mid - 1, temp_buf + mid + 1);

	merge(data, mid + 1, length - mid - 1, temp_buf);
}

//////////////////////////////////////////////////////////////////////////
//
//    merge_sort_inplace -- none recursive inplace version,
//    using STL std::inplace_merge for merging.
//
//    According to my test, this is the slowest of all, 100% slower than the fastest
//    merge_sort, std::inplace_merge seems to be the bottle-neck of the performance.
//
/////////////////////////////////////////////////////////////////////////

template<class T>
void merge_sort_inplace(T data[], int length)
{
	T *data_end = data + length;
	T *first = data;
	int cur_step = 1;

	while (cur_step < length)
	{
		bool end_of_round = false;
		while (first < data_end)
		{
			T *mid = first + cur_step;
			if (mid >= data_end)	// The second part of data does not exist, no need to merge
				break;
			T *last = mid + cur_step;
			if (last >= data_end)
			{
				last = data_end;
				end_of_round = true;	// We have reached the end of the data, merge the two into one
			}

			// merge the two sorted sub-data into one
			std::inplace_merge(first, mid, last);

			if (!end_of_round)
				first = last;
			else
				break;
		}
		if (cur_step >= length)	// The whole data has been sorted
			break;
		else
		{
			cur_step *= 2;
			first = data;
		}
	}
}

#if 0
int main(void)
{
	int data_1[] = { 8, 7, 6, 5, 4, 3, 2, 1 };
	int data_2[] = { 3, 4, 3, 2, 2, 2, 2, 6, 5, 1, 1, 1, 7, 9, 10, 9, 8 };
	int data_3[] = { 7, 5, 6 };
	int data_4[] = { 9, 8 };

	int *datas[] = { data_1, data_2, data_3, data_4 };
	int cnts[] = { 8, 17, 3, 2 };

	int buffer[100];

	for (int i = 0; i < 6; ++i)
	{
		//merge_sort(datas[i], cnts[i]);
		merge_sort_recursive(datas[i], cnts[i], buffer);
		//merge_sort_inplace(datas[i], cnts[i]);
	}

	return 0;
}
#endif


#define N 1000000
int data_raw[N];
int buffer[N];

int main()
{
	srand(17);
	for (int i = 0; i<N; i++)
		data_raw[i] = rand() % 999983;
	data_raw[11] = std::numeric_limits<int>::max() - 11;
	data_raw[N - 11] = (std::numeric_limits<int>::max() >> 4) + 11;
	auto t0 = high_resolution_clock::now();
	merge_sort(data_raw, N);
	auto t1 = high_resolution_clock::now();
	for (int i = N - 1; i>N - 11; --i)
		std::cout << data_raw[i] << ", ";
	std::cout << std::endl << std::endl;

	srand(17);
	for (int i = 0; i<N; i++)
		data_raw[i] = rand() % 999983;
	data_raw[11] = std::numeric_limits<int>::max() - 11;
	data_raw[N - 11] = (std::numeric_limits<int>::max() >> 4) + 11;
	auto t2 = high_resolution_clock::now();
	merge_sort_recursive(data_raw, N, buffer);
	auto t3 = high_resolution_clock::now();
	for (int i = N - 1; i>N - 11; --i)
		std::cout << data_raw[i] << ", ";
	std::cout << std::endl << std::endl;

	srand(17);
	for (int i = 0; i<N; i++)
		data_raw[i] = rand() % 999983;
	data_raw[11] = std::numeric_limits<int>::max() - 11;
	data_raw[N - 11] = (std::numeric_limits<int>::max() >> 4) + 11;
	auto t4 = high_resolution_clock::now();
	merge_sort_inplace(data_raw, N);
	auto t5 = high_resolution_clock::now();
	for (int i = N - 1; i>N - 11; --i)
		std::cout << data_raw[i] << ", ";
	std::cout << std::endl << std::endl;

	srand(17);
	for (int i = 0; i<N; i++)
		data_raw[i] = rand() % 999983;
	data_raw[11] = std::numeric_limits<int>::max() - 11;
	data_raw[N - 11] = (std::numeric_limits<int>::max() >> 4) + 11;
	auto t6 = high_resolution_clock::now();
	std::sort(data_raw, data_raw+N);
	auto t7 = high_resolution_clock::now();
	for (int i = N - 1; i>N - 11; --i)
		std::cout << data_raw[i] << ", ";
	std::cout << std::endl << std::endl;

	std::cout << "Time of merge_sort: " << duration_cast<milliseconds>(t1 - t0).count() << " milliseconds" << std::endl;
	std::cout << "Time of merge_sort_recursive: " << duration_cast<milliseconds>(t3 - t2).count() << " milliseconds" << std::endl;
	std::cout << "Time of merge_sort_inplace: " << duration_cast<milliseconds>(t5 - t4).count() << " milliseconds" << std::endl;
	std::cout << "Time of std::sort: " << duration_cast<milliseconds>(t7 - t6).count() << " milliseconds" << std::endl;

	return 0;
}
