#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

//
// This function is to generate all combinations of choosing m from n items in lexicographic order.
// The return vaule of this function is all index's combinations. This is only a naive none-recursive
// implementation, and performance is rather bad. Actually, this programm is focusing on multi-combination,
// so this function is not important here, as long as it can generate combination of indexes. I implemented
// a quite nice combination generator, please refer to combination algorithm.
//
std::vector<std::vector<int>> combination_idx_nr(int n_items, int m)
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
			auto pr = partial_results.front();
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


/*
    get_multi_comb_cnt() is to select identical objects from multiple groups and to construct a mixed combination.
	Because the objects in the groups are identical, so no need to select individual objects to construct the combination,
	only the number of the objects selected from that group is enough.
	This is useful in solving problems that involves same kind of objects but differ in certain properties, Such as:
	    1) From groups of red balls and blue balls, choose a mixture of N balls.
		2) The classical missionary-and-cannibal problem
		......
*/
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
		std::vector<std::vector<int>> partial_results = get_multi_comb_cnt(item_cnt_grps + 1, n_grps - 1, num_combo - i);
		for (auto pr : partial_results)
		{
			pr.insert(pr.begin(), i);
			results.push_back(pr);
		}
	}
	return results;
}

/*
    get_multi_comb_idx() is to select same kind of objects but with individuality, from multiple groups distinguished
	by certain properties, and to construct a mixed combination. Because the objects in the groups are not identical,
	so you cannot just pick the number of the objects selected from that group to construct a combination, but have to
	choose objects one by one.
    This is useful in solving problems that involves individual objects of the same kind, but differ in certain properties,
	generally, this algorithm often involves with some complex problems, such as:
        1) The expanded missionary-and-cannibal problem, each missionary or cannibal took different time to row the boat
		across the river, so what is the best plan to cross the river in the shortest time?
		2) The expanded knapsack problem, you have to pack certain kinds of items into the knapsack no matter its size and value,
		so, how to achieve the optimal value?
		......
	To achieve the utmost generality, and simplicity, I make the algorithm to return indexes from each group. 
*/
std::vector<std::vector<std::vector<int>>> get_multi_comb_idx(int *grp_cnts, int num_grps, int combo_cnt)
{
	std::vector<std::vector<std::vector<int>>> results;

	if (num_grps == 1)
	{
		if (combo_cnt == 0)
			results.push_back(std::vector<std::vector<int>>(1, std::vector<int>()));
		else if (grp_cnts[0] >= combo_cnt)
		{
			for (auto vi : combination_idx_nr(grp_cnts[0], combo_cnt))
				results.push_back(std::vector<std::vector<int>>(1, vi));
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
				comb_0 = combination_idx_nr(grp_cnts[0], i);
			for (auto c : comb_0)
			{
				for (auto gc : get_multi_comb_idx(grp_cnts + 1, num_grps - 1, combo_cnt - i))
				{
					gc.insert(gc.begin(), c);
					results.push_back(std::move(gc));
				}
			}
		}
	}
	return results;
}


/*
    Well, I suppose you were shocked by the scaring vector<vector<vector<int>>> declaration of the get_multi_comb_idx(),
    this one is a little bit better, ...... vector<vector<int>>. This one uses one bit of the integer to present
    one index in the group. So, obviously, the group cannot have more items than the bits in the integer, usually
    32 bits for an unsigned integer, or 64 bits for an int64_t or long long, this should be quite enough for
    most circumstances.	Ideally, this algorithm could have been implemented using bitset, but, STL has only fixed length
	bitset, although Boost library has a nice dynamic_bitset, I would rather not to introduce another library into this code,
	so, I just implemented it as is using integer types, until future new C++ standard introduce its own dynamic bitset.
*/
std::vector<std::vector<int>> get_multi_comb_idx_bits(int *grp_cnts, int num_grps, int combo_cnt)
{
	std::vector<std::vector<int>> results;

	if (num_grps == 1)
	{
		if (combo_cnt == 0)
			results.push_back(std::vector<int>(1, 0));
		else if (grp_cnts[0] >= combo_cnt)
		{
			for (auto vi : combination_idx_nr(grp_cnts[0], combo_cnt))
			{
				int this_grp_comb = 0;
				for(int i : vi)
					this_grp_comb |= (1 << i);
				results.push_back(std::vector<int>(1, this_grp_comb));
			}
		}
	}
	else if (combo_cnt == 0)
	{
		std::vector<int> temp;
		for (int i = 0; i < num_grps; ++i)
			temp.push_back(0);
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
				comb_0 = combination_idx_nr(grp_cnts[0], i);
				
			for (auto c : comb_0)
			{
				int grp_cnt_0 = 0;
				for(int i : c)
					grp_cnt_0 |= (1<<i);
				for (auto vi : get_multi_comb_idx_bits(grp_cnts + 1, num_grps - 1, combo_cnt - i))
				{
					vi.insert(vi.begin(), grp_cnt_0);
					results.push_back(std::move(vi));
				}
			}
		}
	}
	return results;
}



int main()
{
	int cnts_arr1[] = { 1, 2, 3, 1 };
	std::vector<std::vector<int>> multi_cnts = get_multi_comb_cnt(cnts_arr1, 4, 3);
	for (auto vi : multi_cnts)
	{
		std::cout << "[ ";
		for (auto i : vi)
			std::cout << i << "  ";
		std::cout << "];  " << std::endl;
	}
	std::cout << std::endl;

	int cnts_arr2[] = { 3, 2, 1 };
	std::vector<std::vector<std::vector<int>> > multi_idxs = get_multi_comb_idx(cnts_arr2, 3, 3);
	for (auto vvi : multi_idxs)
	{
		for (auto vi : vvi)
		{
			std::cout << "[ ";
			for (auto i : vi)
				std::cout << i << "  ";
			std::cout << "];  ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::vector<std::vector<int>> multi_idxs_bits = get_multi_comb_idx_bits(cnts_arr2, 3, 3);
	for (auto vi : multi_idxs_bits)
	{
		for (auto i : vi)
		{
			std::cout << "[ ";
			for (unsigned int idx = 0; idx<8*sizeof(int); ++idx)
				if(i & (1 << idx))
					std::cout << idx << "  ";
			std::cout << "];  ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	return 0;
}
