#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

std::vector<std::vector<int>> partition(int N, int start)
{
	std::vector<std::vector<int>> results;
	if (N == 0)
	{
		results.push_back(std::vector<int>());
		return results;
	}
	if (N < start)
		return results;
	for (int i = start; i <= N; ++i)
	{
		if (start < i)
			start = i;
		for (auto x : partition(N - i, start))
		{
			x.insert(x.begin(), i);
			results.push_back(std::move(x));
		}
	}
	return results;
}

template<class CONTAINER=std::vector<int>>
bool next_partition(CONTAINER& cur_part)
{
	auto last = std::prev(cur_part.end());
	if (last == cur_part.begin())
		return false;
	auto second_last = std::prev(last);
	if (*last - *second_last <= 1)
	{
		*second_last += *last;
		cur_part.pop_back();
	}
	else
	{
		++(*second_last);
		--(*last);
		int cur_least = *second_last;
		int remain = *last;
		if(remain >= 2 * cur_least)
		{
			*last = cur_least;
			remain -= cur_least;
			while (remain >= 2 * cur_least)
			{
				cur_part.push_back(cur_least);
				remain -= cur_least;
			}
			cur_part.push_back(remain);
		}
	}
	return true;
}

template<class CONTAINER = std::vector<int>>
bool prev_partition(CONTAINER& cur_part)
{
	if (cur_part.front() == 1 || cur_part.empty())
		return false;

	auto last = std::prev(cur_part.end());
	int remain = 0;
	while (*last == 1 && last != cur_part.begin())
	{
		++remain;
		last = std::prev(last);
		cur_part.pop_back();
	}
	--(*last);
	int cur_max = *last;
	++remain;
	while (remain >= cur_max)
	{
		cur_part.push_back(cur_max);
		remain -= cur_max;
	}
	if (remain)
		cur_part.push_back(remain);

	return true;
}


int main()
{
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "******partition of number " << i << std::endl;
		for (auto vi : partition(i, 1))
		{
			for (int x : vi)
				std::cout << x << ", ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;
	for (auto vi : partition(6, 1))
	{
		for (int i : vi)
			std::cout << i << ", ";
		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;
	for (auto vi : partition(6, 2))
	{
		for (int i : vi)
			std::cout << i << ", ";
		std::cout << std::endl;
	}

	std::cout << std::endl << "*** next partition ***" << std::endl;
	std::vector<int> init_next_part{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	do {
		for (int i : init_next_part)
			std::cout << i << ", ";
		std::cout << std::endl;
	} while (next_partition(init_next_part));

	std::cout << std::endl << "*** prev partition ***" << std::endl;
	std::vector<int> init_prev_part{ 9 };
	do {
		for (int i : init_prev_part)
			std::cout << i << ", ";
		std::cout << std::endl;
	} while (prev_partition(init_prev_part));

	return 0;
}
