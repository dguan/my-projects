#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
#include <exception>
#include <stdexcept>
using namespace std;

/* ------------------ problem 1: knapsack problem ---------------- */

/*
const int N=3;
const int W=10;
const int weight[N]	= {1, 5, 7};//, 4, 20, 8, 11, 15, 6, 3};
const int value[N]	= {5, 8, 10};//, 8, 45, 20, 31, 39, 4, 10};
*/

const int N = 10;
const int W = 30;
const int weight[N] = { 1, 5, 7, 4, 20, 8, 11, 15, 6, 3 };
const int value[N] = { 5, 8, 10, 8, 45, 20, 31, 39, 4, 10 };

typedef struct item {
	int w;
	int v;
	item() = default;// : w(0), v(0) {};
	item(int _w, int _v) : w(_w), v(_v) {};
}Item;

// Naive recursive solution of the knapsack problem, complexity is O(2^n)
vector<Item> knapsack_recur(const vector<Item>& items, int bag_limit)
{
	vector<Item> result;
	if (items.size() == 1)
	{
		if (items[0].w <= bag_limit)
			result.push_back(items[0]);
		return result;
	}
	vector<Item> copy = std::move(items);
	Item cur_item = copy[0];
	copy.erase(copy.begin());
	if (cur_item.w > bag_limit)
		result = knapsack_recur(copy, bag_limit);
	else
	{
		vector<Item> result1 = knapsack_recur(copy, bag_limit - cur_item.w);
		vector<Item> result2 = knapsack_recur(copy, bag_limit);
		int value1 = std::accumulate(result1.begin(), result1.end(), cur_item.v, [&](int a, const Item& x) { return a + x.v; });
		int value2 = std::accumulate(result2.begin(), result2.end(), 0, [&](int a, const Item& x) { return a + x.v; });

		if (value1 > value2)
		{
			result = result1;
			result.push_back(cur_item);
		}
		else
			result = result2;
	}
	return result;
}



// Dynamic Programming solution of the knapsack problem, complexity O(n^2)
vector<Item> knapsack(const vector<Item>& items, int bag_limit)
{
	vector< vector<int> > results(items.size());
	for (auto i = 0u; i<results.size(); ++i)
		results[i].resize(bag_limit + 1);

	for (auto i = 0u; i<items.size(); i++)
	{
		for (int j = 1; j <= bag_limit; j++)
		{
			results[i][j] = (i>0 ? results[i - 1][j] : 0);
			if (j >= items[i].w)
			{
				int temp = items[i].v + (i>0 ? results[i - 1][j - items[i].w] : 0);
				if (temp>results[i][j])
					results[i][j] = temp;
			}
		}
	}

	int max_val = results[items.size() - 1][bag_limit];
	vector<Item> items_in_knapsack;
	int next_item = bag_limit;
	for (int i = static_cast<int>(items.size()) - 1; i >= 0; i--)
	{
		if (max_val>(i - 1 >= 0 ? results[i - 1][next_item] : 0))	// here i has to be signed int
		{
			items_in_knapsack.push_back(items[i]);
			max_val -= items[i].v;
			next_item -= items[i].w;
		}
	}

	return items_in_knapsack;
}

// This knapsack solution's time complexity is still O(n^2), but it has only O(n) space complexity.
// As a price, it can not keep track of the item information, only the maximum value ......
int knapsack_1(const vector<Item>& items, int bag_limit)
{
	vector<int> vals(bag_limit + 1);

	for (auto i = 0u; i<items.size(); ++i)
	{
		for (int j = bag_limit; j >= 1; --j)
		{
			if (items[i].w > j)
				continue;

			int new_val = items[i].v + vals[j - items[i].w];
			if (new_val > vals[j])
				vals[j] = new_val;
		}
	}

	return vals[bag_limit];
}

/* ------------------ problem 2: stick-cutting problem ---------------- */

typedef struct _stick
{
	int length;
	int value;

	_stick(int l, int v) : length(l), value(v) {};
} Stick;

#define TotalLength		20

const int _len_list[] = { 1, 2, 3, 4, 5, 6 };
const int _val_list[] = { 3, 8, 11, 15, 20, 30 };

vector<Stick> cut_stick(const vector<Stick>& sticks, int total_length)
{
	vector<Stick> result;

	vector< vector<int> > vals(sticks.size() + 1);
	for (auto i = 0u; i<vals.size(); ++i)
		vals[i].resize(total_length + 1);

	for (auto i = 1u; i <= sticks.size(); ++i)
	{
		for (int j = 1; j <= total_length; ++j)
		{
			vals[i][j] = vals[i - 1][j];
			if (sticks[i - 1].length > j)
				continue;
			//int new_val = sticks[i-1].value + vals[i-1][j-sticks[i-1].length];
			int new_val = sticks[i - 1].value + vals[i][j - sticks[i - 1].length];
			if (new_val > vals[i][j])
				vals[i][j] = new_val;
		}
	}

	int max_val = vals[sticks.size()][total_length];
	int max_val_idx = total_length;
	while (max_val != 0)
	{
		for (auto i = 0u; i<sticks.size(); ++i)
		{
			if (vals[sticks.size()][max_val_idx - sticks[i].length] == max_val - sticks[i].value)
			{
				result.push_back(sticks[i]);
				max_val -= sticks[i].value;
				max_val_idx -= sticks[i].length;
				//break;
			}
		}
	}
	return result;
}

vector<Stick> cut_stick1(const vector<Stick>& sticks, int total_length, int& max_value)
{
	vector<int> vals(total_length + 1);

	for (auto i = 0u; i<sticks.size(); ++i)
	{
		for (int j = 1; j <= total_length; ++j)
		{
			if (sticks[i].length > j)
				continue;
			//int new_val = sticks[i-1].value + vals[i-1][j-sticks[i-1].length];
			int new_val = sticks[i].value + vals[j - sticks[i].length];
			if (new_val > vals[j])
				vals[j] = new_val;
		}
	}
	max_value = vals[total_length];

	vector<Stick> result;
	int max_val = vals[total_length];
	int max_val_idx = total_length;
	while (max_val != 0)
	{
		for (auto i = 0u; i<sticks.size(); ++i)
		{
			if (vals[max_val_idx - sticks[i].length] == max_val - sticks[i].value)
			{
				result.push_back(sticks[i]);
				max_val -= sticks[i].value;
				max_val_idx -= sticks[i].length;
				//break;
			}
		}
	}
	return result;
}

//int seq[] = {1, -1, 2, -3, 4, -5, 6, -7};
//int seq[] = {5, 4, 3, 2, 1};
int seq[] = { 1, 2, 3, 4, 5 };
//int seq[] = {5, 4, 3, 2, 1, 6};
//int seq[] = {5, 4};
//int seq[] = {5, 6};
//int seq[] = {5};
//int seq[] = {3, 2, 2, 2, 2, 2, 4, 1, 1, 1, 1, 1, 1, 5};
//int seq[] = {6, 4, 3, 7, 1, 2, 8};
//int seq[] = {15, 14, 5, 4, 13, 10, 11, 0, 1, 16, 0, 3, 5, 12, 3, 1, 4};
//int seq[] = {5, 4, 13, 10, 11, 0, 1, 16, 0, 3, 5, 12, 3, 1, 4};

int LIS(int data[], int len)
{
	int *lis = new int[len];
	if (lis == NULL)
		throw std::runtime_error("Error allocate memory");
	/*
	int *max = new int[len];
	if(max == NULL)
	throw std::runtime_error("Error allocate memory");
	memset(max, std::numeric_limits<int>::min(), len*sizeof(int));
	*/
	//int lis[100];
	//int max[100];
	//for(int i=0; i<100; ++i)
	//	lis[i] = 1;//memset(lis, 1, 100*sizeof(int));
	//	memset(max, std::numeric_limits<int>::min(), len*sizeof(int));

	for (int i = 0; i <= len - 1; i++)
	{
		lis[i] = 1;
		for (int j = 0; j<i; j++)
		{
			if (data[i]>data[j] && lis[j] + 1>lis[i])
				lis[i] = lis[j] + 1;
		}
	}
	int max_len = *std::max_element(lis, lis + len);
	delete[] lis;
	return max_len;
}




int main(void)
{
	vector<Item> its;
	for (int i = 0; i<N; ++i)
		its.push_back(Item(weight[i], value[i]));


	// recursive solution
	vector<Item> max_val_items_recur = knapsack_recur(its, W);

	int max_val_recur = 0;
	for (auto i = 0u; i<max_val_items_recur.size(); ++i)
		max_val_recur += max_val_items_recur[i].v;

	for (auto i = 0u; i<max_val_items_recur.size(); ++i)
		cout << "[" << max_val_items_recur[i].w << ", " << max_val_items_recur[i].v << "] - ";
	cout << endl << "Max value returned by naive Recursive Function is: " << max_val_recur << endl << endl;



	vector<Item> max_val_items = knapsack(its, W);

	int max_val = 0;
	for (auto i = 0u; i<max_val_items.size(); ++i)
		max_val += max_val_items[i].v;

	for (auto i = 0u; i<max_val_items.size(); ++i)
		cout << "[" << max_val_items[i].w << ", " << max_val_items[i].v << "] - ";
	cout << endl << "Max value returned by Dynamic Programming is: " << max_val << endl;

	max_val = knapsack_1(its, W);
	cout << endl << "Max value returned by knapsack_1 is: " << max_val << endl << endl;

	vector<Stick> sticks;
	for (auto i = 0u; i<sizeof(_len_list) / sizeof(_len_list[0]); ++i)
		sticks.push_back(Stick(_len_list[i], _val_list[i]));

	vector<Stick> cut = cut_stick(sticks, TotalLength);

	cut = cut_stick1(sticks, TotalLength, max_val);
	cout << "Maximum cutting value is: " << max_val << endl;
	for (auto i = 0u; i<cut.size(); ++i)
		cout << "[" << cut[i].length << ", " << cut[i].value << "]  ";
	cout << endl << endl;

	cout << "The longest Increasing Sequence's length is: " << LIS(seq, sizeof(seq) / sizeof(seq[0])) << endl;

	return 0;
}


