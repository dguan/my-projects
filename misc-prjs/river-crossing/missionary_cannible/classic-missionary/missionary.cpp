/*
The Problem

The Missionaries and Cannibals problem is a classic AI puzzle that can be defined as follows :

On one bank of a river are three missionaries and three cannibals.There is one boat available that
can hold up to two people and that they would like to use to cross the river.If the cannibals ever
outnumber the missionaries on either of the river¡¯s banks, the missionaries will get eaten.

How can the boat be used to safely carry all the missionaries and cannibals across the river ?
*/

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <queue>
#include <chrono>
using namespace std::chrono;

struct state
{
	int status[4];		//mLeft, cLeft, mRight, cRight
	bool boat_side;		//boat's side, false for left side, true for right side
	std::string movs;	// the moves till this state
	state(int mL, int cL, int mR, int cR, bool s, std::string mv) : boat_side(s), movs(mv)
	{
		status[0] = mL, status[1] = cL, status[2] = mR, status[3] = cR;
	};
};

inline bool	is_safe_mov(int nml, int ncl, int nmr, int ncr)
{
	return (!nml || nml >= ncl) && (!nmr || nmr >= ncr);
}

inline bool solution_found(const state& st, int boat_volume)
{
	return (st.status[0] + st.status[1] <= boat_volume) && (st.boat_side == false);
}

std::vector<std::vector<int>> get_combo(const int *item_cnts, int n_items, int max_num_combo)
{
	std::vector<std::vector<int>> results;

	if (n_items == 1)
	{
		if (item_cnts[0] >= max_num_combo)
			results.emplace_back(std::vector<int>(1, max_num_combo));
		return results;
	}

	for (int i = std::min(max_num_combo, item_cnts[0]); i >= 0; --i)
	{
		std::vector<std::vector<int>> partial_results = std::move(get_combo(item_cnts + 1, n_items - 1, max_num_combo - i));
		for (auto& pr : partial_results)
		{
			pr.insert(pr.begin(), i);
			results.push_back(pr);
		}
	}
	return results;
}

std::vector<state> get_safe_movs(const state& st, int boat_volume, const std::vector<std::tuple<int, int, bool>>& used_states)
{
	std::vector<state> results;
	std::vector<std::vector<int>> possible_combos;
	if (st.boat_side)
		for (int i = 1; i <= boat_volume; ++i)
			for (auto& gc : get_combo(st.status + 2, 2, i))
				possible_combos.push_back(std::move(gc));
	else
		for (int i = boat_volume; i >= 1;  --i)
			for (auto& gc : get_combo(st.status, 2, i))
				possible_combos.push_back(std::move(gc));

	for (auto& pc : possible_combos)
	{
		int nml = st.boat_side ? st.status[0] + pc[0] : st.status[0] - pc[0];	// num of missionaries on the left side
		int ncl = st.boat_side ? st.status[1] + pc[1] : st.status[1] - pc[1];	// num of cannibals on the left side
		int nmr = st.boat_side ? st.status[2] - pc[0] : st.status[2] + pc[0];	// num of missionaries on the right side
		int ncr = st.boat_side ? st.status[3] - pc[1] : st.status[3] + pc[1];	// num of cannibals on the right side

		if (!is_safe_mov(nml, ncl, nmr, ncr))
			continue;
		bool found = false;
		for (auto& us : used_states)
		{
			if (!st.boat_side == std::get<2>(us) && nml == std::get<0>(us) && ncl == std::get<1>(us))
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			std::string mov(" ");
			for (int i = 0; i < pc[0]; ++i)
				mov += 'm';
			for (int i = 0; i < pc[1]; ++i)
				mov += 'c';
			results.emplace_back(nml, ncl, nmr, ncr, !st.boat_side, st.movs + mov + (st.boat_side ? "<- " : "-> "));
		}
	}
	return results;
}

std::string find_solution_dfs(const state& cur_state, std::vector<std::tuple<int, int, bool>>& used_states, int boat_volume)
{
	if (solution_found(cur_state, boat_volume))
	{
		std::string s = cur_state.movs + " ";
		for (int i = 0; i < cur_state.status[0]; ++i)
			s += 'm';
		for (int i = 0; i < cur_state.status[1]; ++i)
			s += 'c';
		return s + "-> ";
	}
	else
	{
		used_states.emplace_back(cur_state.status[0], cur_state.status[1], cur_state.boat_side);
		std::vector<state> safe_movs = std::move(get_safe_movs(cur_state, boat_volume, used_states));

		for (unsigned int i = 0; i < safe_movs.size(); ++i)
		{
			std::string solution = find_solution_dfs(safe_movs[i], used_states, boat_volume);
			if (solution != "No Solution")
				return solution;
		}
		return "No Solution";
	}
}

std::string find_solution_bfs(std::queue<state>& cur_states, std::vector<std::tuple<int, int, bool>>& used_states, int boat_volume)
{
	while(! cur_states.empty())
	{
		state st = cur_states.front();
		cur_states.pop();
		used_states.emplace_back(st.status[0], st.status[1], st.boat_side);
		for (auto& gc : get_safe_movs(st, boat_volume, used_states))
		{
			if (solution_found(gc, boat_volume))
			{
				std::string s = gc.movs + " ";
				for (int i = 0; i < gc.status[0]; ++i)
					s += 'm';
				for (int i = 0; i < gc.status[1]; ++i)
					s += 'c';
				return s + "-> ";
			}
			else
				cur_states.push(std::move(gc));
		}
	}
	return "No Solution";
}

int main(void)
{
	const int num_missionaries = 3;
	const int num_cannibals = 3;
	const int boat_volume = 2;
	
	//const int num_missionaries = 6;
	//const int num_cannibals = 6;
	//const int boat_volume = 3;

	//const int num_missionaries = 100;
	//const int num_cannibals = 100;
	//const int boat_volume = 4;

	//const int num_missionaries = 100;
	//const int num_cannibals = 99;
	//const int boat_volume = 2;

	if (num_missionaries < 0 || num_cannibals < 0 || (num_missionaries != 0 && num_missionaries < num_cannibals))
	{
		std::cout << "The number of missionaries and the number of cannibals must not be negative," << std::endl;
		std::cout << "and, the number of missionaries must not be less than the number of cannibals." << std::endl;
		return 1;
	}
	else
	{
		std::vector<std::tuple<int, int, bool>> used_states;
		auto t0 = high_resolution_clock::now();
		std::cout << find_solution_dfs(state(num_missionaries, num_cannibals, 0, 0, false, ""), used_states, boat_volume) << std::endl;
		auto t1 = high_resolution_clock::now();
		std::cout << "The DFS solution for (M:" << num_missionaries << ", C:" << num_cannibals << ", BoatVolume:" << boat_volume << ") costs " << duration_cast<milliseconds>(t1-t0).count() << " milliseconds." <<std::endl << std::endl;

		used_states.clear();
		std::queue<state> q_states;
		q_states.push(state(num_missionaries, num_cannibals, 0, 0, false, ""));
		auto t2 = high_resolution_clock::now();
		std::cout << find_solution_bfs(q_states, used_states, boat_volume) << std::endl;
		auto t3 = high_resolution_clock::now();
		std::cout << "The BFS solution for (M:" << num_missionaries << ", C:" << num_cannibals << ", BoatVolume:" << boat_volume << ") costs " << duration_cast<milliseconds>(t3 - t2).count() << " milliseconds." << std::endl << std::endl;
	}
	return 0;
}

