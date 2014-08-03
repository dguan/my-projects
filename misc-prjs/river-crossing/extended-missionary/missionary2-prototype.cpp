#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <tuple>
#include <numeric>
#include <algorithm>
#include <cassert>

template<class BiItor>
bool next_combination_idx(int n, BiItor comb_first, BiItor comb_last)
{
	int k = std::distance(comb_first, comb_last);
	assert(n >= k && k > 0);

	BiItor cur_pos = comb_last;
	--cur_pos;
	int chg_max = n - 1;
	while (true)
	{
		typedef decltype(*comb_first + *comb_first) T;
		auto cur_idx = *cur_pos + 1;
		if (chg_max >= cur_idx)
		{
			while (cur_pos != comb_last)
				*cur_pos++ = cur_idx++;
			return true;
		}
		else
		{
			if (cur_pos != comb_first)
			{
				--cur_pos;
				--chg_max;
			}
			else
				return false;
		}
	}
}

std::vector<std::vector<int>> get_all_combs_idx(int n, int k)
{
	std::vector<std::vector<int>> all_combs;
	std::vector<int> cur_comb(k);
	std::iota(cur_comb.begin(), cur_comb.end(), 0);
	do {
		all_combs.push_back(cur_comb);
	} while (next_combination_idx(n, cur_comb.begin(), cur_comb.end()));

	return all_combs;
}

std::vector<std::vector<std::vector<int>>> get_multi_comb_idx(int *grp_cnts, int num_grps, int combo_cnt)
{
	std::vector<std::vector<std::vector<int>>> results;

	if (num_grps == 1)
	{
		if (combo_cnt == 0)
			results.push_back(std::vector<std::vector<int>>(1, std::vector<int>()));
		else if (grp_cnts[0] >= combo_cnt)
		{
			for (auto vi : get_all_combs_idx(grp_cnts[0], combo_cnt))
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
				comb_0 = get_all_combs_idx(grp_cnts[0], i);
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

struct state
{
	std::vector<int> mLeft;
	std::vector<int> cLeft;
	std::vector<int> mRight;
	std::vector<int> cRight;
	bool boat_side;			// boat's side, false for left side, true for right side
	std::string movs;		// the moves till this state
	int time;				// the time till this state
	state(std::vector<int> mL, std::vector<int> cL, std::vector<int> mR, std::vector<int> cR, bool side, std::string mv, int tm);
};

state::state(std::vector<int> mL, std::vector<int> cL, std::vector<int> mR, std::vector<int> cR, bool side, std::string mv, int tm)
: mLeft(mL), cLeft(cL), mRight(mR), cRight(cR), boat_side(side), movs(mv), time(tm)
{}

typedef std::tuple<std::vector<int>, std::vector<int>, int> SaveState;
typedef std::vector<SaveState> SaveStateVector;

inline bool	is_safe_mov(int nml, int ncl, int nmr, int ncr)
{
	return (!nml || nml >= ncl) && (!nmr || nmr >= ncr);
}

inline bool solution_found(const state& st, int boat_volume)
{
	return (st.mLeft.size()+st.cLeft.size() <= (unsigned int)boat_volume) && (st.boat_side == false);
}

std::vector<state> get_safe_movs(const state& st, int boat_volume, const SaveStateVector& used_states_left, const SaveStateVector& used_states_right)
{
	std::vector<state> results;
	std::vector<std::vector<std::vector<int>>> possible_combos;
	int grps[2];
	if (st.boat_side)
	{
		grps[0] = st.mRight.size();
		grps[1] = st.cRight.size();
		for (int i = 1; i <= boat_volume; ++i)
			for (auto gc : get_multi_comb_idx(grps, 2, i))
				possible_combos.push_back(std::move(gc));
	}
	else
	{
		grps[0] = st.mLeft.size();
		grps[1] = st.cLeft.size();
		for (int i = boat_volume; i >= 1; --i)
			for (auto gc : get_multi_comb_idx(grps, 2, i))
				possible_combos.push_back(std::move(gc));
	}
	for (auto pc : possible_combos)
	{
		std::vector<int> ml = st.mLeft;
		std::vector<int> cl = st.cLeft;
		std::vector<int> mr = st.mRight;
		std::vector<int> cr = st.cRight;

		int min_m_time = std::numeric_limits<int>::max();
		int min_c_time = std::numeric_limits<int>::max();

		if (st.boat_side)
		{
			std::vector<int> temp;
			for (int i : pc[0])
			{
				ml.push_back(mr[i]);
				temp.push_back(mr[i]);
			}
			for (auto i : temp)
				mr.erase(std::find(mr.begin(), mr.end(), i));
			if (!temp.empty())
				min_m_time = *std::min_element(temp.begin(), temp.end());

			temp.clear();
			for (int i : pc[1])
			{
				cl.push_back(cr[i]);
				temp.push_back(cr[i]);
			}
			for (auto i : temp)
				cr.erase(std::find(cr.begin(), cr.end(), i));
			if (!temp.empty())
				min_c_time = *std::min_element(temp.begin(), temp.end());
		}
		else
		{
			std::vector<int> temp;
			for (int i : pc[0])
			{
				mr.push_back(ml[i]);
				temp.push_back(ml[i]);
			}
			for (auto i : temp)
				ml.erase(std::find(ml.begin(), ml.end(), i));
			if (!temp.empty())
				min_m_time = *std::min_element(temp.begin(), temp.end());

			temp.clear();
			for (int i : pc[1])
			{
				cr.push_back(cl[i]);
				temp.push_back(cl[i]);
			}
			for (auto i : temp)
				cl.erase(std::find(cl.begin(), cl.end(), i));
			if (!temp.empty())
				min_c_time = *std::min_element(temp.begin(), temp.end());
		}
		std::sort(ml.begin(), ml.end());
		std::sort(cl.begin(), cl.end());
		std::sort(mr.begin(), mr.end());
		std::sort(cr.begin(), cr.end());

		if (!is_safe_mov(ml.size(), cl.size(), mr.size(), cr.size()))
			continue;
		bool found = false;
		const SaveStateVector& working_vec = st.boat_side ? used_states_left : used_states_right;
		for (auto us : working_vec)
		{
			if (ml.size() == std::get<0>(us).size() && cl.size() == std::get<1>(us).size() && \
				std::equal(ml.begin(), ml.end(), std::get<0>(us).begin()) && \
				std::equal(cl.begin(), cl.end(), std::get<1>(us).begin()) )
			{
				if (st.time + std::min(min_m_time, min_c_time) >= std::get<2>(us))
				{
					found = true;
					break;
				}
			}
		}
		if (!found)
		{
			std::string mov(" ");
			if (st.boat_side)
			{
				for (auto i : pc[0])
					mov = mov + 'm' + std::to_string(st.mRight[i]);
				for (auto i : pc[1])
					mov = mov + 'c' + std::to_string(st.cRight[i]);
			}
			else
			{
				for (auto i : pc[0])
					mov = mov + 'm' + std::to_string(st.mLeft[i]);
				for (auto i : pc[1])
					mov = mov + 'c' + std::to_string(st.cLeft[i]);
			}
			results.emplace_back(ml, cl, mr, cr, !st.boat_side, st.movs + mov + (st.boat_side ? "<- " : "-> "), st.time + std::min(min_m_time, min_c_time));
		}
	}
	return results;
}

std::vector<std::tuple<int, std::string>> find_solution_dfs(const state& cur_st, int boat_volume, SaveStateVector& used_states_left, SaveStateVector& used_states_right)
{
	std::vector<std::tuple<int, std::string>> solutions;
	if (solution_found(cur_st, boat_volume))
	{
		int min_m_time = std::numeric_limits<int>::max();
		int min_c_time = std::numeric_limits<int>::max();
		std::string s = cur_st.movs + " ";
		for (int i : cur_st.mLeft)
		{
			s = s + 'm' + std::to_string(i);
			if (i < min_m_time)
				min_m_time = i;
		}
		for (int i : cur_st.cLeft)
		{
			s = s + 'c' + std::to_string(i);
			if (i < min_c_time)
				min_c_time = i;
		}
		solutions.emplace_back(cur_st.time + std::min(min_m_time, min_c_time),  s + "-> ");
		return solutions;
	}
	else
	{
		SaveStateVector& working_vec = cur_st.boat_side ? used_states_right : used_states_left;
		bool found = false;
		for (auto itr = working_vec.begin(); itr != working_vec.end(); ++itr)
		{
			if (cur_st.mLeft.size() == std::get<0>(*itr).size() && \
				cur_st.cLeft.size() == std::get<1>(*itr).size() && \
				std::equal(cur_st.mLeft.begin(), cur_st.mLeft.end(), std::get<0>(*itr).begin()) && \
				std::equal(cur_st.cLeft.begin(), cur_st.cLeft.end(), std::get<1>(*itr).begin()))
			{
				if (cur_st.time < std::get<2>(*itr))
					working_vec.erase(itr);
				else
					found = true;
				break;
			}
		}
		if (!found)
			working_vec.emplace_back(cur_st.mLeft, cur_st.cLeft, cur_st.time);
		for (auto gsm : get_safe_movs(cur_st, boat_volume, used_states_left, used_states_right))
		{
			for(auto tpl : find_solution_dfs(gsm, boat_volume, used_states_left, used_states_right))
				solutions.push_back(std::move(tpl));
		}
	}
	return solutions;
}

std::vector<std::tuple<int, std::string>> find_solution_bfs(std::queue<state>& cur_states, int boat_volume, SaveStateVector& used_states_left, SaveStateVector& used_states_right)
{
	std::vector<std::tuple<int, std::string>> solutions;
	while (!cur_states.empty())
	{
		state st = cur_states.front();
		cur_states.pop();
		SaveStateVector& working_vec = st.boat_side ? used_states_right : used_states_left;
		bool found = false;
		for (auto itr = working_vec.begin(); itr != working_vec.end(); ++itr)
		{
			if (st.mLeft.size() == std::get<0>(*itr).size() && \
				st.cLeft.size() == std::get<1>(*itr).size() && \
				std::equal(st.mLeft.begin(), st.mLeft.end(), std::get<0>(*itr).begin()) && \
				std::equal(st.cLeft.begin(), st.cLeft.end(), std::get<1>(*itr).begin()))
			{
				if (st.time < std::get<2>(*itr))
					working_vec.erase(itr);
				else
					found = true;
				break;
			}
		}
		if (!found)
			working_vec.emplace_back(st.mLeft, st.cLeft, st.time);

		for (auto gc : get_safe_movs(st, boat_volume, used_states_left, used_states_right))
		{
			if (solution_found(gc, boat_volume))
			{
				int min_m_time = std::numeric_limits<int>::max();
				int min_c_time = std::numeric_limits<int>::max();
				std::string s = gc.movs + " ";
				for (int i : gc.mLeft)
				{
					s = s + 'm' + std::to_string(i);
					if (i < min_m_time)
						min_m_time = i;
				}
				for (int i : gc.cLeft)
				{
					s = s + 'c' + std::to_string(i);
					if (i < min_c_time)
						min_c_time = i;
				}
				solutions.push_back(std::tuple<int, std::string>(gc.time + std::min(min_m_time, min_c_time), s + "-> "));
			}
			else
				cur_states.push(std::move(gc));
		}
	}
	return solutions;
}


int main(void)
{
	for (auto vi : get_all_combs_idx(5, 3))
	{
		for (auto i : vi)
			std::cout << i << ",  ";
		std::cout << std::endl;
	}

	int grp_cnts[] = {3, 2, 2};
	for (auto vvi : get_multi_comb_idx(grp_cnts, 3, 4))
	{
		for (auto vi : vvi)
		{
			std::cout << "[ ";
			for (auto i : vi)
				std::cout << i << ", ";
			std::cout << "]   ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << "******************** DFS algorithm *********************" << std::endl << std::endl;

	// Both DFS and BFS give out correct answer 29, but,
	// DFS has only 60 solutions, while BFS has 8100!
	//const std::vector<int> missionaries{ 4, 5, 6 };
	//const std::vector<int> cannibles{ 2, 3, 5 };
	//const int boat_volume = 2;

	// Both DFS and BFS give out correct answer 28, but, DFS solutions
	// increased to 284, while BFS solutions jumped to 944784! So it will
	// take tons of time even to print all these solutions.
	//const std::vector<int> missionaries{ 4, 5, 6, 7 };
	//const std::vector<int> cannibles{ 2, 3, 5 };
	//const int boat_volume = 2;

	// There is no solution when M=C=4 and boat_volume=2
	//const std::vector<int> missionaries{ 6, 8, 10, 12 };
	//const std::vector<int> cannibles{ 3, 5, 7, 9 };
	//const int boat_volume = 2;

	// Never run BFS algorithm on this!
	// Best time 11, 368 DFS solutions.
	const std::vector<int> missionaries{ 2, 4, 5, 6 };
	const std::vector<int> cannibles{ 1, 2, 3, 4 };
	const int boat_volume = 3;

	// Total 1584 DFS solutions
	//const std::vector<int> missionaries{ 2, 4, 5, 6, 8 };
	//const std::vector<int> cannibles{ 1, 2, 3, 4, 5 };
	//const int boat_volume = 3;

	SaveStateVector used_states_left, used_states_right;
	auto dfs_solutions = find_solution_dfs(state(missionaries, cannibles, std::vector<int>(), std::vector<int>(), false, "", 0), boat_volume, used_states_left, used_states_right);

	std::sort(dfs_solutions.begin(), dfs_solutions.end(), [&](const std::tuple<int, std::string>& a, const std::tuple<int, std::string>& b) { return std::get<0>(a) < std::get<0>(b); });
	for (auto x : dfs_solutions)
		std::cout << std::get<0>(x) << " :: " << std::get<1>(x) << std::endl << std::endl;

/*
	std::cout << std::endl << "******************** BFS algorithm *********************" << std::endl << std::endl;

	std::queue<state> q_states;
	used_states_left.clear();
	used_states_right.clear();
	q_states.push(state(missionaries, cannibles, std::vector<int>(), std::vector<int>(), false, "", 0));
	auto bfs_solutions = find_solution_bfs(q_states, boat_volume, used_states_left, used_states_right);

	std::sort(bfs_solutions.begin(), bfs_solutions.end(), [&](const std::tuple<int, std::string>& a, const std::tuple<int, std::string>& b) { return std::get<0>(a) < std::get<0>(b); });
	for (auto x : bfs_solutions)
		std::cout << std::get<0>(x) << " :: " << std::get<1>(x) << std::endl << std::endl;
*/
	if (!dfs_solutions.empty())
	{
		std::cout << "***** Total " << dfs_solutions.size() << " DFS solutions *****" << std::endl << std::endl;
		//std::cout << "***** Total " << bfs_solutions.size() << " BFS solutions *****" << std::endl << std::endl;
		std::cout << "The best DFS solution is: " << std::get<0>(dfs_solutions[0]) << " :: " << std::get<1>(dfs_solutions[0]) << std::endl << std::endl;
		//std::cout << "The best BFS solution is: " << std::get<0>(bfs_solutions[0]) << " :: " << std::get<1>(bfs_solutions[0]) << std::endl << std::endl;
	}
	else
		std::cout << "No Solution." << std::endl;

	return 0;
}
