#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
#include <utility>
#include <exception>
#include <stdexcept>


typedef struct _rod
{
	int length;
	int value;

	_rod(int l, int v) : length(l), value(v) {};
} Rod;

#define TotalLength		20

const int _len_list[] = { 1, 2, 3, 4, 5, 6 };
const int _val_list[] = { 3, 8, 11, 15, 20, 30 };

std::vector<Rod> cut_rod(const std::vector<Rod>& rods, int total_length)
{
	std::vector<Rod> result;

	std::vector<std::vector<int>> vals(rods.size() + 1);
	for (auto i = 0u; i<vals.size(); ++i)
		vals[i].resize(total_length + 1);

	for (auto i = 1u; i <= rods.size(); ++i)
	{
		for (int j = 1; j <= total_length; ++j)
		{
			vals[i][j] = vals[i - 1][j];
			if (rods[i - 1].length > j)
				continue;
			//int new_val = rods[i-1].value + vals[i-1][j-rods[i-1].length];
			int new_val = rods[i - 1].value + vals[i][j - rods[i - 1].length];
			if (new_val > vals[i][j])
				vals[i][j] = new_val;
		}
	}

	int max_val = vals[rods.size()][total_length];
	int max_val_idx = total_length;
	while (max_val != 0)
	{
		for (auto i = 0u; i<rods.size(); ++i)
		{
			if (vals[rods.size()][max_val_idx - rods[i].length] == max_val - rods[i].value)
			{
				result.push_back(rods[i]);
				max_val -= rods[i].value;
				max_val_idx -= rods[i].length;
				//break;
			}
		}
	}
	return result;
}

std::vector<Rod> cut_rod1(const std::vector<Rod>& rods, int total_length, int& max_value)
{
	std::vector<int> vals(total_length + 1);

	for (auto i = 0u; i < rods.size(); ++i)
	{
		for (int j = 1; j <= total_length; ++j)
		{
			if (rods[i].length > j)
				continue;
			//int new_val = rods[i-1].value + vals[i-1][j-rods[i-1].length];
			int new_val = rods[i].value + vals[j - rods[i].length];
			if (new_val > vals[j])
				vals[j] = new_val;
		}
	}
	max_value = vals[total_length];

	std::vector<Rod> result;
	int max_val = vals[total_length];
	int max_val_idx = total_length;
	while (max_val != 0)
	{
		for (auto i = 0u; i<rods.size(); ++i)
		{
			if (vals[max_val_idx - rods[i].length] == max_val - rods[i].value)
			{
				result.push_back(rods[i]);
				max_val -= rods[i].value;
				max_val_idx -= rods[i].length;
				//break;
			}
		}
	}
	return result;
}


int main(void)
{
	std::vector<Rod> rods;
	for (auto i = 0u; i<sizeof(_len_list) / sizeof(_len_list[0]); ++i)
		rods.push_back(Rod(_len_list[i], _val_list[i]));

	std::vector<Rod> cut = cut_rod(rods, TotalLength);

	int max_val = 0;
	cut = cut_rod1(rods, TotalLength, max_val);
	std::cout << "Maximum cutting value is: " << max_val << std::endl;
	for (auto i = 0u; i<cut.size(); ++i)
		std::cout << "[" << cut[i].length << ", " << cut[i].value << "]  ";
	std::cout << std::endl << std::endl;

	return 0;
}


