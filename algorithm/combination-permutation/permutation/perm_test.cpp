#include "permutation.h"
#include <iostream>
#include <string>
#include <chrono>

using namespace std::chrono;

//
// Code from cppreference.com
// http://en.cppreference.com/w/cpp/algorithm/next_permutation
// And this is an explaination of the code
// http://stackoverflow.com/questions/11483060/stdnext-permutation-implementation-explanation
//
template<class BidirIt>
bool next_perm(BidirIt first, BidirIt last)
{
	if (first == last) return false;
	BidirIt i = last;
	if (first == --i) return false;

	while (1)
	{
		BidirIt i1, i2;

		i1 = i;
		if (*--i < *i1)	// Find the position of the first neighboring position [...i, i1...] is in ascending order
		{
			i2 = last;
			while (!(*i < *--i2))	// Finding the first number greater than *i,(and is the greatest)in the right to be swapped with *i
				;
			std::iter_swap(i, i2);	// Swap *i and *i2
			std::reverse(i1, last);	// reverse the position after i till end
			return true;
		}
		if (i == first)
		{
			std::reverse(first, last);
			return false;
		}
	}
}

// for testing simulated P(n, k)
#include "../combination/combination.h"

int main()
{
	PermIdx pi1(4, 3);

	int cnt = 0;
	do {
		std::cout << "Next permutation is: ";
		for (auto i : pi1.get_cur_perm())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	} while (pi1.next_perm_idx());
	std::cout << "Altogether " << cnt << " permutations" << std::endl << std::endl;

	cnt = 0;
	do {
		std::cout << "Previous permutation is: ";
		for (auto i : pi1.get_cur_perm())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	} while (pi1.prev_perm_idx());
	std::cout << "Altogether " << cnt << " permutations" << std::endl << std::endl;

	std::string str("ABCD");
	PermSeq<char> ps1(4, 3, &str[0]);
	cnt = 0;
	do {
		std::cout << "Next permutation is: ";
		for (auto c : ps1.get_cur_perm())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	} while (ps1.next_perm_seq());
	std::cout << "Altogether " << cnt << " permutations" << std::endl << std::endl;

	cnt = 0;
	do {
		std::cout << "Previous permutation is: ";
		for (auto c : ps1.get_cur_perm())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	} while (ps1.prev_perm_seq());
	std::cout << "Altogether " << cnt << " permutations" << std::endl << std::endl;

	cnt = 0;
	do {
		std::cout << "Next permutation is: " << str << std::endl;
		++cnt;
	} while (next_perm(str.begin(), str.end()));
	std::cout << "Altogether " << cnt << " permutations" << std::endl << std::endl;


	std::cout << std::endl << "--------- permutation benchmarking for 11 digits ----------" << std::endl << std::endl;

	std::vector<int> vec_i1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	auto t0 = high_resolution_clock::now();
	while (std::next_permutation(vec_i1.begin(), vec_i1.end()));
	auto t1 = high_resolution_clock::now();
	std::cout << "Time of std::next_permutation is: " << duration_cast<milliseconds>(t1 - t0).count() << " milliseconds" << std::endl;

	std::vector<int> vec_i2{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	auto t2 = high_resolution_clock::now();
	while (next_perm(vec_i2.begin(), vec_i2.end()));
	auto t3 = high_resolution_clock::now();
	std::cout << "Time of next_perm is: " << duration_cast<milliseconds>(t3 - t2).count() << " milliseconds" << std::endl;

	PermIdx pi2(12, 9);
	//PermIdx pi2(11, 10);
	//PermIdx pi2(11, 11);
	//PermIdx pi2(8, 8);
	auto t4 = high_resolution_clock::now();
	while (pi2.next_perm_idx());
	auto t5 = high_resolution_clock::now();
	std::cout << "Time of this next_perm_idx is: " << duration_cast<milliseconds>(t5 - t4).count() << " milliseconds" << std::endl;


	CombIdx ci(12, 9);
	auto t6 = high_resolution_clock::now();
	do {
		std::vector<int> cur_comb(std::move(ci.get_cur_comb()));
		while(std::next_permutation(cur_comb.begin(), cur_comb.end()));
	} while(ci.next_comb_idx());
	auto t7 = high_resolution_clock::now();
	std::cout << "Time of simulatd P(12, 9) is: " << duration_cast<milliseconds>(t7 - t6).count() << " milliseconds" << std::endl;

	return 0;
}
