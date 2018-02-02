#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iterator>
#include <utility>
#include <chrono>
#include <algorithm>

using namespace std;

// Choose K items from n, return index sequence.
bool next_comb(unsigned int n, std::vector<unsigned int>& in_out_comb)
{
	unsigned int change_position = 0;
	while (true)
	{
		unsigned int cur_pos = in_out_comb.size() - 1 - change_position;
		unsigned int cur_idx = in_out_comb[cur_pos] + 1;
		if (cur_idx < n - change_position)
		{
			while(cur_pos < in_out_comb.size())
				in_out_comb[cur_pos++] = cur_idx++;
			return true;
		}
		else
		{
			++change_position;
			if (change_position == in_out_comb.size())
				return false;
		}
	}
}

// STL library freaking rocks, std::next_permutation can be more than
// 10 times faster than this next_perm. Yet, this next_perm can do 
// P(n, k) while std::next_permutation can only do P(n, n). 
bool next_perm(unsigned int n, std::vector<unsigned int>& in_out_perm)
{
	std::vector<int> lookup_table(n);
	for (auto x : in_out_perm)
		lookup_table[x] = 1;
	unsigned int change_position = 0;
	while (true)
	{
		unsigned int cur_pos = in_out_perm.size() - 1 - change_position;
		unsigned int cur_idx = in_out_perm[cur_pos];
		lookup_table[cur_idx++] = 0;
		while (cur_idx < n)
		{
			if (!lookup_table[cur_idx])
				break;
			else
				++cur_idx;
		}
		if (cur_idx < n)
		{
			lookup_table[in_out_perm[cur_pos]] = 0;
			in_out_perm[cur_pos++] = cur_idx;
			lookup_table[cur_idx] = 1;
			unsigned int lookup_table_idx = 0;
			while (cur_pos < in_out_perm.size())
			{
				while (lookup_table[lookup_table_idx] && lookup_table_idx < n)
					++lookup_table_idx;
				in_out_perm[cur_pos++] = lookup_table_idx++;
			}
			return true;
		}
		else
		{
			++change_position;
			if (change_position == in_out_perm.size())
				return false;
		}
	}
}


/* basic recursive routine that return all combinations */
template<class T>
vector< vector<T> > combination_obj(T items[], int n, int m, bool lexical_order)
{
    vector<vector<T>> results;
	if (m <= 0 || n < m)
		return results;
	if (m == 1)
    {
        for(int i=0; i<n; ++i)
            results.emplace_back(vector<T>(1, items[i]));
        return results;
    }
    
    for(int i=0; i<=n-m; ++i)
    {
        T& x = items[i];
		for (vector<T>& t : combination_obj(&items[i + 1], n - i - 1, m - 1, lexical_order))
        {
			if (lexical_order)
				t.insert(t.begin(), x);
			else
				t.push_back(x);
			results.emplace_back(std::move(t));
        }
    }
    return results;
}

vector<vector<int>> combination_seq(int n, int m, int start_idx, bool lexical_order)
{
	vector<vector<int>> results;
	if (m <= 0 || n < m)
		return results;
	if (m == 1)
	{
		for (int i = start_idx; i< start_idx + n; ++i)
			results.emplace_back(1, i);
		return results;
	}

	for (int i = start_idx; i <= start_idx + n - m; ++i)
	{
		for (auto& vi : combination_seq(start_idx + n - i - 1, m - 1, i+1, lexical_order))
		{
			if (lexical_order)
				vi.insert(vi.begin(), i);
			else
				vi.push_back(i);
			results.emplace_back(std::move(vi));
		}
	}
	return results;
}

// This is in lexicographic order
std::vector<std::vector<int>> combination_nr_que_seq(int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<int>>();
	std::vector<std::vector<int>> combos;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			combos.emplace_back(1, i);
		return combos;
	}
	std::queue<std::vector<int>> partial_results;
	for (int i = 0; i<n_items - m + 1; ++i)
		partial_results.emplace(1, i);
	int cur_depth = m - 1;

	while (cur_depth >= 1)
	{
		int cnt = partial_results.size();
		for (int i = 0; i < cnt; ++i)
		{
			auto& pr = partial_results.front();
			partial_results.pop();
			for (int j = m - cur_depth; j < n_items - cur_depth + 1; ++j)
			{
				if (j>pr.back())
				{
					std::vector<int> temp(pr);
					temp.push_back(j);
					if (cur_depth>1)
						partial_results.push(std::move(temp));
					else
						combos.push_back(std::move(temp));
				}
			}
		}
		--cur_depth;
	}
	return combos;
}

// This is NOT in lexicographic order
std::vector<std::vector<int>> combination_nr_stk_seq(int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<int>>();
	std::vector<std::vector<int>> combos;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			combos.emplace_back(1, i);
		return combos;
	}
	std::stack<std::vector<int>> partial_results;
	for (int i = 0; i<= n_items - m; ++i)
		partial_results.emplace(1, i);

	while (!partial_results.empty())
	{
		auto& pr = partial_results.top();
		partial_results.pop();
		for (unsigned int j = pr.size(); j <= (unsigned)(n_items - m + pr.size()); ++j)
		{
			if (j>(unsigned)pr.back())
			{
				std::vector<int> temp(pr);
				temp.push_back(j);
				if (pr.size() < (unsigned)(m - 1))
					partial_results.push(std::move(temp));
				else
					combos.push_back(std::move(temp));
			}
		}
	}
	return combos;
}

// This is in lexicographic order
template<class T>
std::vector<std::vector<T>> combination_nr_que_obj(const T items[], int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<T>>();
	std::vector<std::vector<T>> combos;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			combos.emplace_back(1, items[i]);
		return combos;
	}
	std::queue<std::vector<T>> partial_results;
	for (int i = 0; i<n_items - m + 1; ++i)
		partial_results.emplace(1, items[i]);
	int cur_depth = m - 1;

	while (cur_depth >= 1)
	{
		int cnt = partial_results.size();
		for (int i = 0; i < cnt; ++i)
		{
			auto& pr = partial_results.front();
			partial_results.pop();
			for (int j = m - cur_depth; j < n_items - cur_depth + 1; ++j)
			{
				if (items[j]>pr.back())
				{
					std::vector<T> temp(pr);
					temp.push_back(items[j]);
					if (cur_depth>1)
						partial_results.push(std::move(temp));
					else
						combos.push_back(std::move(temp));
				}
			}
		}
		--cur_depth;
	}
	return combos;
}

// This is NOT in lexicographic order
template<class T>
std::vector<std::vector<T>> combination_nr_stk_obj(const T items[], int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<T>>();
	std::vector<std::vector<T>> combos;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			combos.emplace_back(1, items[i]);
		return combos;
	}
	std::stack<std::vector<T>> partial_results;
	for (int i = 0; i<n_items - m + 1; ++i)
		partial_results.emplace(1, items[i]);

	while(!partial_results.empty())
	{
		auto& pr = partial_results.top();
		partial_results.pop();
		for (unsigned int j = pr.size(); j <= (unsigned)(n_items - m + pr.size()); ++j)
		{
			if (items[j]>pr.back())
			{
				std::vector<T> temp(pr);
				temp.push_back(items[j]);
				if (pr.size() < (unsigned)(m - 1))
					partial_results.push(std::move(temp));
				else
					combos.push_back(std::move(temp));
			}
		}
	}
	return combos;
}

template<class T>
vector< vector<T> > permutation(T items[], int n, int m, bool lexical_order)
{
	vector<vector<T>> results;
	if (m <= 0 || n < m)
		return results;
	if (m == 1)
	{
		for (int i = 0; i<n; ++i)
			results.emplace_back(vector<T>(1, items[i]));
		return results;
	}

	for (int i = 0; i < n; ++i)
	{
		T& x = items[i];
		if (std::find(items, items+i, x) != items+i)
			continue;
		vector<T> sub_set(items, items + i);
		std::copy(items + i + 1, items + n, std::back_inserter(sub_set));

		vector<vector<T>> temp = permutation(&sub_set[0], n-1, m - 1, lexical_order);
		for (vector<T>& t : temp)
		{
			if (lexical_order)
				t.insert(t.begin(), x);
			else
				t.push_back(x);
			results.emplace_back(std::move(t));
		}
	}
	return results;
}

// This is in lexicographic order
std::vector<std::vector<int>> permutation_nr_que_seq(int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<int>>();
	std::vector<std::vector<int>> permus;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			permus.emplace_back(1, i);
		return permus;
	}
	std::queue<std::vector<int>> partial_results;
	for (int i = 0; i<n_items; ++i)
		partial_results.emplace(1, i);
	int cur_depth = m - 1;

	while (cur_depth >= 1)
	{
		int cnt = partial_results.size();
		for (int i = 0; i < cnt; ++i)
		{
			auto& pr = partial_results.front();
			partial_results.pop();
			for (int j = 0; j < n_items; ++j)
			{
				if (std::find(pr.begin(), pr.end(), j) == pr.end())
				{
					std::vector<int> temp(pr);
					temp.push_back(j);
					if (cur_depth>1)
						partial_results.push(std::move(temp));
					else
						permus.push_back(std::move(temp));
				}
			}
		}
		--cur_depth;
	}
	return permus;
}

// This is NOT in lexicographic order
std::vector<std::vector<int>> permutation_nr_stk_seq(int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<int>>();
	std::vector<std::vector<int>> permus;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			permus.emplace_back(1, i);
		return permus;
	}
	std::stack<std::vector<int>> partial_results;
	for (int i = 0; i<n_items; ++i)
		partial_results.emplace(1, i);

	while(!partial_results.empty())
	{
		auto& pr = partial_results.top();
		partial_results.pop();
		for (int j = 0; j < n_items; ++j)
		{
			if (std::find(pr.begin(), pr.end(), j) == pr.end())
			{
				std::vector<int> temp(pr);
				temp.push_back(j);
				if (pr.size() < (unsigned)(m-1))
					partial_results.push(std::move(temp));
				else
					permus.push_back(std::move(temp));
			}
		}
	}
	return permus;
}

// This is in lexicographic order
template<class T>
std::vector<std::vector<T>> permutation_nr_que_obj(const T items[], int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<T>>();
	std::vector<std::vector<T>> permus;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			permus.emplace_back(1, items[i]);
		return permus;
	}
	std::queue<std::vector<T>> partial_results;
	for (int i = 0; i<n_items; ++i)
		partial_results.emplace(1, items[i]);
	int cur_depth = m - 1;

	while (cur_depth >= 1)
	{
		int cnt = partial_results.size();
		for (int i = 0; i < cnt; ++i)
		{
			auto& pr = partial_results.front();
			partial_results.pop();
			for (int j = 0; j < n_items; ++j)
			{
				if (std::find(pr.begin(), pr.end(), items[j]) == pr.end())
				{
					std::vector<int> temp(pr);
					temp.push_back(items[j]);
					if (cur_depth>1)
						partial_results.push(std::move(temp));
					else
						permus.push_back(std::move(temp));
				}
			}
		}
		--cur_depth;
	}
	return permus;
}

// This is NOT in lexicographic order
template<class T>
std::vector<std::vector<T>> permutation_nr_stk_obj(const T items[], int n_items, int m)
{
	if (m <= 0 || n_items < m)
		return std::vector<std::vector<T>>();
	std::vector<std::vector<T>> permus;
	if (m == 1)
	{
		for (int i = 0; i < n_items; ++i)
			permus.emplace_back(1, items[i]);
		return permus;
	}
	std::stack<std::vector<T>> partial_results;
	for (int i = 0; i<n_items; ++i)
		partial_results.emplace(1, items[i]);
	while(!partial_results.empty())
	{
		auto& pr = partial_results.top();
		partial_results.pop();
		for (int j = 0; j < n_items; ++j)
		{
			if (std::find(pr.begin(), pr.end(), items[j]) == pr.end())
			{
				std::vector<int> temp(pr);
				temp.push_back(items[j]);
				if (pr.size() < m - 1)
					partial_results.push(std::move(temp));
				else
					permus.push_back(std::move(temp));
			}
		}
	}
	return permus;
}

// This is to choose num_combo items from multiple groups, like to choose one or two from missionary and cannibles.
// You do not have to choose single items, because if they are in the same group, they are all the same.
// what you need is just a count of each group.
std::vector<std::vector<int>> get_multi_comb_cnt(int item_cnt_grps[], int n_grps, int num_combo)
{
	std::vector<std::vector<int>> results;

	if (n_grps == 1)
	{
		if (item_cnt_grps[0] >= num_combo)
			results.emplace_back(std::vector<int>(1, num_combo));
		return results;
	}

	for (int i = std::min(num_combo, item_cnt_grps[0]); i >= 0; --i)
	{
		std::vector<std::vector<int>> partial_results = std::move(get_multi_comb_cnt(item_cnt_grps + 1, n_grps - 1, num_combo - i));
		for (auto& pr : partial_results)
		{
			pr.insert(pr.begin(), i);
			results.push_back(pr);
		}
	}
	return results;
}

// This is to choose combo_cnt items from multiple groups, This is an enhanced combination,
// which works on multiple groups of items, and each item has its specialty that can not be
// selected as a number, but you have to choose individuals from multiple groups.
std::vector<std::vector<std::vector<int>>> get_multi_comb_idx(int *grp_cnts, int num_grps, int combo_cnt)
{
	std::vector<std::vector<std::vector<int>>> results;

	if (num_grps == 1)
	{
		if (combo_cnt == 0)
			results.push_back(std::vector<std::vector<int>>(1, std::vector<int>()));
		else if (grp_cnts[0] >= combo_cnt)
		{
			for (auto& vi : combination_nr_que_seq(grp_cnts[0], combo_cnt))
				results.push_back(std::vector<std::vector<int>>(1, vi));
			return results;
		}
	}
	else if (combo_cnt == 0)
	{
		std::vector<std::vector<int>> temp;
		for (int i = 0; i < num_grps; ++i)
			temp.push_back(std::vector<int>());
		results.push_back(std::move(temp));
		return results;
	}
	else
	{
		for (int i = std::min(combo_cnt, grp_cnts[0]); i >= 0; --i)
		{
			std::vector<std::vector<int>> comb_0;
			if (i == 0)
				comb_0.push_back(std::vector<int>());
			else
				comb_0 = std::move(combination_nr_que_seq(grp_cnts[0], i));
			for (auto c : comb_0)
			{
				for (auto& gc : get_multi_comb_idx(grp_cnts + 1, num_grps - 1, combo_cnt - i))
				{
					gc.insert(gc.begin(), c);
					results.push_back(std::move(gc));
				}
			}
		}
	}
	return results;
}




int main(void)
{
	//int data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	// Be careful! you should not define data using C string literals like char data[]={"cookbook"};
	char data[] = { 'c', 'o', 'o', 'k', 'b', 'o', 'o', 'k' };
	//auto combo = combination(data, sizeof(data) / sizeof(data[0]), 3);
	//vector<vector<decltype(data[0] + data[0])>> permu = permutation(data, sizeof(data) / sizeof(data[0]), 1);
	auto t0 = chrono::high_resolution_clock::now();
	auto combo = std::move(combination_obj(data, sizeof(data) / sizeof(data[0]), sizeof(data) / sizeof(data[0]), true));
	auto permu = std::move(permutation(data, sizeof(data) / sizeof(data[0]), sizeof(data) / sizeof(data[0]), true));
	auto t1 = chrono::high_resolution_clock::now();

	for (auto& x : permu)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}
	cout << "Altogether " << permu.size() << " permutations" << endl;

	for (auto& x : combo)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}
	cout << "Altogether " << combo.size() << " combinations" << endl;

	cout << "Time spent: " << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() / 1000.0 << " seconds" << endl;

	int mc[2] = { 3, 3 };
	std::vector<std::vector<int>> combo2 = std::move(get_multi_comb_cnt(mc, 2, 2));
	std::vector<std::vector<int>> combo1 = std::move(get_multi_comb_cnt(mc, 2, 1));

	for (auto& vi : combo2)
	{
		for (auto i : vi)
			std::cout << i;
		std::cout << std::endl;
	}
	for (auto& vi : combo1)
	{
		for (auto i : vi)
			std::cout << i;
		std::cout << std::endl;
	}

	std::vector<std::vector<std::string>> peoples = { { "Angela", "Beatrice", "Clara" }, { "John", "Ken", "Loyd", "Mathew" } };

	int cnts_arr[] = {3, 2, 1};
	std::vector<std::vector<std::vector<int>> > ppls = std::move(get_multi_comb_idx(cnts_arr, 3, 3));
	for (auto& vvi : ppls)
	{
		for (auto& vi : vvi)
		{
			std::cout << "[ ";
			for (auto i : vi)
				std::cout << i << ' ';
			std::cout << "], ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	int dt1[] = { 5, 4, 3, 2, 1 };
	int dt2[] = { 1, 2, 3, 4, 5 };

	auto c1 = std::move(combination_obj(dt1, sizeof(dt1) / sizeof(dt1[0]), 3, true));
	auto c2 = std::move(combination_seq(sizeof(dt1) / sizeof(dt1[0]), 3, 0, true));
	auto c3 = std::move(combination_nr_que_obj(dt2, sizeof(dt2) / sizeof(dt2[0]), 3));
	auto c4 = std::move(combination_nr_stk_obj(dt2, sizeof(dt2) / sizeof(dt2[0]), 3));
	auto c5 = std::move(combination_nr_que_seq(sizeof(dt2) / sizeof(dt2[0]), 3));
	auto c6 = std::move(combination_nr_stk_seq(sizeof(dt2) / sizeof(dt2[0]), 3));

	//auto c1 = std::move(permutation_obj(dt1, sizeof(dt1) / sizeof(dt1[0]), 2));
	//auto c2 = std::move(permutation_seq(sizeof(dt1) / sizeof(dt1[0]), 2, 0));
	//auto c3 = std::move(permutation_nr_que_obj(dt2, sizeof(dt2) / sizeof(dt2[0]), 2));
	//auto c4 = std::move(permutation_nr_stk_obj(dt2, sizeof(dt2) / sizeof(dt2[0]), 2));
	//auto c5 = std::move(permutation_nr_que_seq(sizeof(dt2) / sizeof(dt2[0]), 2));
	//auto c6 = std::move(permutation_nr_stk_seq(sizeof(dt2) / sizeof(dt2[0]), 2));

	for (auto& x : c1)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}
	cout << endl;
	for (auto& x : c2)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}
	cout << endl;
	for (auto& x : c3)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}
	cout << endl;
	for (auto& x : c4)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}
	cout << endl;
	for (auto& x : c5)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}
	cout << endl;
	for (auto& x : c6)
	{
		for (auto xx : x)
			cout << xx << ", ";
		cout << endl;
	}

	std::vector<unsigned int> nc{ 0, 1, 2 };
	while (next_comb(5, nc))
	{
		std::cout << "Next combination is: ";
		for (auto ui : nc)
			std::cout << ui << ", ";
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;

	std::vector<unsigned int> np{ 0, 1, 2 };
	while (next_perm(4, np))
	{
		std::cout << "Next permutation is: ";
		for (auto ui : np)
			std::cout << ui << ", ";
		std::cout << std::endl;
	}


	std::string str { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	auto t2 = chrono::high_resolution_clock::now();
	while (std::next_permutation(str.begin(), str.end()));
	auto t3 = chrono::high_resolution_clock::now();
	cout << "Time spent: " << chrono::duration_cast<chrono::milliseconds>(t3 - t2).count() / 1000.0 << " seconds" << endl;

	std::vector<unsigned int> ui2{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	auto t4 = chrono::high_resolution_clock::now();
	while (next_perm(9, ui2));
	auto t5 = chrono::high_resolution_clock::now();
	cout << "Time spent: " << chrono::duration_cast<chrono::milliseconds>(t5 - t4).count() / 1000.0 << " seconds" << endl;

    return 0;
}
