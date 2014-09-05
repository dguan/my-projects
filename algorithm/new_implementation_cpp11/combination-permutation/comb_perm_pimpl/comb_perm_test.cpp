#include "combination.h"
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
			while (!(*i < *--i2))	// Finding the first position from right that is no less than *i (that's the smallest possible increment)
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


int main()
{
	CombIdx ci(8, 4);

	int cnt = 0;
	while (ci.next_comb_idx())
	{
		std::cout << "Next combination is: ";
		for (auto i : ci.get_cur_comb())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt+1 << " combinations" << std::endl << std::endl;

	cnt = 0;
	while (ci.prev_comb_idx())
	{
		std::cout << "Previous combination is: ";
		for (auto i : ci.get_cur_comb())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " combinations" << std::endl << std::endl;

	char seq[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	CombSeq<char> cs(6, 3, &seq[0]);

	cnt = 0;
	while (cs.next_comb_seq())
	{
		std::cout << "Next combination is: ";
		for (auto c : cs.get_cur_comb())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " combinations" << std::endl << std::endl;

	cnt = 0;
	while (cs.prev_comb_seq())
	{
		std::cout << "Previous combination is: ";
		for (auto c : cs.get_cur_comb())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " combinations" << std::endl << std::endl;


	std::cout << "---------------------- permutations ------------------------" << std::endl;

	PermIdx pi1(4, 3);

	cnt = 0;
	while (pi1.next_perm_idx())
	{
		std::cout << "Next permutation is: ";
		for (auto i : pi1.get_cur_perm())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " permutations" << std::endl << std::endl;

	cnt = 0;
	while (pi1.prev_perm_idx())
	{
		std::cout << "Previous permutation is: ";
		for (auto i : pi1.get_cur_perm())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " permutations" << std::endl << std::endl;


	PermSeq<char> ps1(4, 3, &seq[0]);
	cnt = 0;
	while (ps1.next_perm_seq())
	{
		std::cout << "Next permutation is: ";
		for (auto c : ps1.get_cur_perm())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " permutations" << std::endl << std::endl;

	cnt = 0;
	while (ps1.prev_perm_seq())
	{
		std::cout << "Previous permutation is: ";
		for (auto c : ps1.get_cur_perm())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " permutations" << std::endl << std::endl;

	cnt = 0;
	std::string str("ABCD");
	while (next_perm(str.begin(), str.end()))
	{
		std::cout << "Next permutation is: " << str << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt + 1 << " permutations" << std::endl << std::endl;

	std::vector<int> vec_i1{ 0, 1, 2, 3, 4, 5, 6, 7 };
	auto t0 = high_resolution_clock::now();
	while (std::next_permutation(vec_i1.begin(), vec_i1.end()));
	auto t1 = high_resolution_clock::now();
	std::cout << "Time of std::next_permutation is: " << duration_cast<milliseconds>(t1 - t0).count() << " milliseconds" << std::endl;

	std::vector<int> vec_i2{ 0, 1, 2, 3, 4, 5, 6, 7 };
	auto t2 = high_resolution_clock::now();
	while (next_perm(vec_i2.begin(), vec_i2.end()));
	auto t3 = high_resolution_clock::now();
	std::cout << "Time of next_perm is: " << duration_cast<milliseconds>(t3 - t2).count() << " milliseconds" << std::endl;

	PermIdx pi2(8, 8);
	auto t4 = high_resolution_clock::now();
	while (pi2.next_perm_idx());
	auto t5 = high_resolution_clock::now();
	std::cout << "Time of this next_perm_idx is: " << duration_cast<milliseconds>(t5 - t4).count() << " milliseconds" << std::endl;

	return 0;
}
