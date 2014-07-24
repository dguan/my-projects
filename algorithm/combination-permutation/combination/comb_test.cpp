#include "combination.h"
#include <iostream>
#include <string>
#include <cstring>
#include <chrono>

using namespace std::chrono;


int main()
{
	CombIdx ci(6, 4);

	int cnt = 0;
	do {
		std::cout << "Next combination is: ";
		for (auto i : ci.get_cur_comb())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	} 	while (ci.next_comb_idx());
	std::cout << "Altogether " << cnt << " combinations" << std::endl << std::endl;

	cnt = 0;
	do {
		std::cout << "Previous combination is: ";
		for (auto i : ci.get_cur_comb())
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	} 	while (ci.prev_comb_idx());
	std::cout << "Altogether " << cnt << " combinations" << std::endl << std::endl;

	char seq[] = "abcdefg";
	CombSeq<char> cs(seq, seq+strlen(seq), 4);

	cnt = 0;
	do {
		std::cout << "Next combination is: ";
		for (auto c : cs.get_cur_comb())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	} while (cs.next_comb_seq());
	std::cout << "Altogether " << cnt << " combinations" << std::endl << std::endl;

	cnt = 0;
	do {
		std::cout << "Previous combination is: ";
		for (auto c : cs.get_cur_comb())
			std::cout << c << ", ";
		std::cout << std::endl;
		++cnt;
	} while (cs.prev_comb_seq());
	std::cout << "Altogether " << cnt << " combinations" << std::endl << std::endl;


	std::cout << std::endl << "-- benchmarking of C(25, 10)=3268760 vs STL next_permutation P(10, 10)=3628800 --" << std::endl << std::endl;

	std::vector<int> vec_i1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	auto t0 = high_resolution_clock::now();
	while (std::next_permutation(vec_i1.begin(), vec_i1.end()));
	auto t1 = high_resolution_clock::now();
	std::cout << "Time of std::next_permutation is: " << duration_cast<milliseconds>(t1 - t0).count() << " milliseconds" << std::endl;

	CombIdx cBench(25, 10);
	auto t2 = high_resolution_clock::now();
	while (cBench.next_comb_idx());
	auto t3 = high_resolution_clock::now();
	std::cout << "Time of next_comb_idx is: " << duration_cast<milliseconds>(t3 - t2).count() << " milliseconds" << std::endl;


	return 0;
}
