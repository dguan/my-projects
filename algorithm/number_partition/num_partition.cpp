#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

std::vector<std::vector<int>> partition_inc(int N, int cur_min)
{
	std::vector<std::vector<int>> results;
	if (N == 0)
	{
		results.push_back(std::vector<int>());
		return results;
	}
	if (N < cur_min)
		return results;
	for (int i = cur_min; i <= N; ++i)
	{
		if (cur_min < i)
			cur_min = i;
		for (auto x : partition_inc(N - i, cur_min))
		{
			x.insert(x.begin(), i);
			results.push_back(std::move(x));
		}
	}
	return results;
}

std::vector<std::vector<int>> partition_dec(int N, int cur_max)
{
	std::vector<std::vector<int>> results;
	if (N == 0)
	{
		results.push_back(std::vector<int>());
		return results;
	}
	for (int i = cur_max; i >= 1; --i)
	{
		if (N > 2*i)
			cur_max = i;
		else
			cur_max = N - i;
		for (auto x : partition_dec(N-i, cur_max))
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
	if (cur_part.empty() || last == cur_part.begin())
		return false;
	auto second_last = std::prev(last);
	if (*last - *second_last <= 1)
	{
		*second_last += *last;
		cur_part.pop_back();
	}
	else if (*last < *second_last * 2 + 3)
	{
		++(*second_last);
		--(*last);
	}
	else
	{
		++(*second_last);
		int cur_least = *second_last;
		int remain = --(*last) - cur_least;
		*last = cur_least;
		while (remain >= cur_least)
		{
			cur_part.push_back(cur_least);
			remain -= cur_least;
		}
		cur_part.back() += remain;
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
		std::cout << "*** incremental partition of number " << i << " ***" << std::endl;
		for (auto vi : partition_inc(i, 1))
		{
			for (int x : vi)
				std::cout << x << ", ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "*** decremental partition of number " << i << " ***" << std::endl;
		for (auto vi : partition_dec(i, i))
		{
			for (int x : vi)
				std::cout << x << ", ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << "*** test next partition ***" << std::endl;
	std::vector<int> init_next_part{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	do {
		for (int i : init_next_part)
			std::cout << i << ", ";
		std::cout << std::endl;
	} while (next_partition(init_next_part));

	std::cout << std::endl << "*** test prev partition ***" << std::endl;
	std::vector<int> init_prev_part{ 9 };
	do {
		for (int i : init_prev_part)
			std::cout << i << ", ";
		std::cout << std::endl;
	} while (prev_partition(init_prev_part));

	return 0;
}
