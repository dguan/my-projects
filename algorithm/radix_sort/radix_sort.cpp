#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
#include <algorithm>
#include <exception>
#include <stdexcept>

#include <cstdlib>
#include <cstring>

#include <chrono>
using namespace std::chrono;

//////////////////////////////////////////////////////////////////////////////////////
//
//    Radix Sort
//    Radix sort is cool, it is fast and stable, the drawbacks are that it requires
//    an O(n) temporary working space plus the extra space for counting sort, 
//    and, it can not work on floating point numbers. Unlike counting sort, this
//    algorithm works well on large data range integers.
//
//    Note that this code currently does not compile in Visual Studio 2013 because that
//    visual Studio 2013 does not support 'constexpr' yet. 
//
//////////////////////////////////////////////////////////////////////////////////////

// order==false for ascending, order==true for descending
// msd_lsd==false for most-significant-digit sort, msd_lsd==true for least-significant-digit sort
// For integer sorting, it makes no sense to sort from MSD to LSD, just keep this option here,
// MSD sort is useflu when sorting strings. 
template <class T, bool order=false, bool msd_lsd=false> T *radix_sort(T data[], int count, T *buff)
{
	if (count <= 0)
		return nullptr;
	T *p1 = data;
	T *p2 = buff;

	// I am using byte as counting radix
	constexpr int counting_size = std::numeric_limits<unsigned char>::max() + 1;
	int cnts[counting_size];

	// radix_rounds is the number of bytes in an integeral type
	constexpr int radix_rounds = sizeof(T);
	constexpr int radix_start = msd_lsd ? radix_rounds - 1 : 0;
	constexpr int radix_end = msd_lsd ? 0 : radix_rounds - 1;

	int rnds = radix_start;
	while (true)
	{
		memset(cnts, 0, sizeof(int)*counting_size);
		for (int i = 0; i<count; ++i)
		{
			int x = (order ? counting_size - 1 - ((p1[i] >> rnds * 8) & 0xff) : ((p1[i] >> rnds * 8) & 0xff));
			++cnts[x];
		}
		for (int i = 1; i<counting_size; ++i)
			cnts[i] += cnts[i - 1];
		for (int i = count - 1; i >= 0; --i)
		{
			int x = (order ? counting_size - 1 - ((p1[i] >> rnds * 8) & 0xff) : ((p1[i] >> rnds * 8) & 0xff));
			p2[--cnts[x]] = p1[i];
		}
		// switch the data and the buffer, and move to next byte
		std::swap(p1, p2);

		if (msd_lsd == true)		// MSD sort
		{
			if (rnds > radix_end)
				--rnds;
			else
				break;
		}
		else		// LSD sort
		{
			if (rnds < radix_end)
				++rnds;
			else
				break;
		}
	}
	// If the integer has even-number(2,4,8,...) bytes, the sorted data will be in the original array
	// If the integer has odd-number bytes(very unlikely), the sorted data will be in the buffer provided
	return p1;
}


constexpr int N = 1000000;
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
	int *p = radix_sort(data_raw, N, buffer);
	auto t1 = high_resolution_clock::now();
	for (int i = N - 1; i>N - 11; --i)
		std::cout << p[i] << ", ";
	std::cout << std::endl << std::endl;


	srand(17);
	for (int i = 0; i<N; i++)
		data_raw[i] = rand() % 999983;
	data_raw[11] = std::numeric_limits<int>::max() - 11;
	data_raw[N - 11] = (std::numeric_limits<int>::max() >> 4) + 11;
	auto t2 = high_resolution_clock::now();
	std::sort(std::begin(data_raw), std::end(data_raw));
	auto t3 = high_resolution_clock::now();
	for (int i = N - 1; i>N - 11; --i)
		std::cout << data_raw[i] << ", ";
	std::cout << std::endl << std::endl;

	std::cout << "Time of radix_sort: " << duration_cast<milliseconds>(t1 - t0).count() << " milliseconds" << std::endl;
	std::cout << "Time of std::sort: " << duration_cast<milliseconds>(t3 - t2).count() << " milliseconds" << std::endl;

	return 0;
}

