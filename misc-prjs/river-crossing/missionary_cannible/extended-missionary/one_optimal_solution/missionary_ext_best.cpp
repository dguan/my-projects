#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <cassert>

template<class BiIter> bool next_combination_idx(int n, BiIter comb_first, BiIter comb_last)
{
	int k = std::distance(comb_first, comb_last);
	assert(n >= k && k > 0);	// Obviously if k==0 there is no next combination

	BiIter cur_pos = comb_last;
	--cur_pos;
	int chg_max = n - 1;
	while (true)
	{
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

std::vector<std::vector<int>> get_multi_comb_idx_bits(int *grp_cnts, int num_grps, int combo_cnt)
{
	std::vector<std::vector<int>> results;

	if (num_grps == 1)
	{
		if (combo_cnt == 0)
			results.push_back(std::vector<int>(1, 0));
		else if (grp_cnts[0] >= combo_cnt)
		{
			for (auto vi : get_all_combs_idx(grp_cnts[0], combo_cnt))
			{
				int this_grp_comb = 0;
				for (int i : vi)
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
				comb_0 = get_all_combs_idx(grp_cnts[0], i);

			for (auto c : comb_0)
			{
				int grp_cnt_0 = 0;
				for (int i : c)
					grp_cnt_0 |= (1 << i);
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


// Both DFS and BFS give out correct answer 20, but,
// DFS has only 30 solutions, while BFS has 8100!
// Make sure to have the 2 tables sorted first!
//const std::vector<int> missionaries{ 4, 5, 6 };
//const std::vector<int> cannibals{ 1, 2, 3 };
//const int boat_volume = 2;

// Both DFS and BFS give out correct answer 20, but,
// DFS has only 30 solutions, while BFS has 8100!
// Make sure to have the 2 tables sorted first!
//const std::vector<int> missionaries{ 4, 5, 6 };
//const std::vector<int> cannibals{ 1, 2, 3 };
//const int boat_volume = 2;

// Both DFS and BFS give out correct answer 28, but, DFS solutions
// increased to 264, while BFS solutions jumped to 944784! So it will
// take tons of time even to print all these solutions.
//const std::vector<int> missionaries{ 4, 5, 6, 7 };
//const std::vector<int> cannibals{ 2, 3, 5 };
//const int boat_volume = 2;

// There is no solution when M=C=4 and boat_volume=2
//const std::vector<int> missionaries{ 6, 8, 10, 12 };
//const std::vector<int> cannibals{ 3, 5, 7, 9 };
//const int boat_volume = 2;

// Best time 11, 368 DFS solutions.
//const std::vector<int> missionaries{ 2, 4, 5, 6 };
//const std::vector<int> cannibals{ 1, 2, 3, 4 };
//const int boat_volume = 3;

// Total 1584 DFS solutions, best time 15
const std::vector<int> missionaries{ 2, 4, 5, 6, 8 };
const std::vector<int> cannibals{ 1, 2, 3, 4, 5 };
const int boat_volume = 3;

// Total 73086 DFS solutions, best time 9, one of the best solution is:
// m2m4c1c2-> m2c1<- m2m5c1c3-> m2c1<- m2m5c1c4-> m5c1<- m6m8c1c5-> m6c1<- m5m6c1c6->
//const std::vector<int> missionaries{ 2, 4, 5, 5, 6, 8 };
//const std::vector<int> cannibals{ 1, 2, 3, 4, 5, 6 };
//const int boat_volume = 4;

// Best time 11, Best solution: m2m4c1c2-> m2c1<- m2m5c1c3-> m2c1<-
// m2m5c1c4-> m2c1<- m2m6c1c5-> m2c1<- m2m8m9c1-> c1<- c1c6c7->
// Try this with some patience.
//const std::vector<int> missionaries{ 2, 4, 5, 5, 6, 8, 9 };
//const std::vector<int> cannibals{ 1, 2, 3, 4, 5, 6, 7 };
//const int boat_volume = 4;

struct state
{
	unsigned int mLeft;
	unsigned int cLeft;
	unsigned int mRight;
	unsigned int cRight;
	bool boat_side;			// boat's side, false for left side, true for right side
	std::string movs;		// the moves till this state
	int time;				// the time till this state
	state(unsigned int mL, unsigned int cL, unsigned int mR, unsigned int cR, bool side, std::string mv, int tm)
		: mLeft(mL), cLeft(cL), mRight(mR), cRight(cR), boat_side(side), movs(mv), time(tm) {};
};

typedef std::unordered_map<unsigned long long, int> UsedStateTable;

const int INTEGER_BITS = sizeof(int)* 8;// Total bits in an integer, maximum elements in an integer should be INTEGER_BITS-1
static int g_Missionary_N;				// number of missionaries
static int g_Cannible_N;				// number of cannibals, this may be less than the missionaries

static int g_BestTime = std::numeric_limits<int>::max();
static std::string g_BestSolution;

unsigned char BIT_CNT[256];		// number of ones in a char, used to check ones in integers quickly
inline void init_bit_cnt_table(unsigned char table[])	// initialize bit-count table
{
	unsigned char bit_cnt_4bits[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
	for (int i = 0; i < 16; ++i)
	for (int j = 0; j < 16; ++j)
		table[i * 16 + j] = bit_cnt_4bits[i] + bit_cnt_4bits[j];
}
inline int count_ones(unsigned int x)	// count the ones in an unsigned integer
{
	int total_bits = 0;
	for (unsigned int i = 0; i < sizeof(int); ++i)
	{
		total_bits += BIT_CNT[x & 0xff];
		x >>= 8;
	}
	return total_bits;
}

inline bool	is_safe_mov(int nml, int ncl, int nmr, int ncr)	// check if a move safe or not
{
	return (!nml || nml >= ncl) && (!nmr || nmr >= ncr);
}

inline bool solution_found(const state& st, int boat_volume)	// check if we have found a solution
{
	return (count_ones(st.mLeft)+count_ones(st.cLeft) <= boat_volume) && (st.boat_side == false);
}

// By checking bit_order, convert the bit index in real order in missionary or cannibal table index
std::vector<int> bit_index_to_index(unsigned int bit_order, int max_bits, int bit_index)
{
	std::vector<int> result;

	std::vector<int> int_index;	// change bit_index's bit index into integer index to lookup the bit_order

	unsigned int bit_mask = 1;
	unsigned int max_bit_mask = (1 << max_bits);
	int bit_pos = 0;
	while (bit_mask < max_bit_mask)
	{
		if (bit_mask & bit_index)
			int_index.push_back(bit_pos);
		bit_mask <<= 1;
		++bit_pos;
	}

	bit_mask = 1;
	bit_pos = 0;
	unsigned int cnt = 0;
	int bit_idx_cnt = -1;
	while (cnt < int_index.size())
	{
		int idx = int_index[cnt];
		while (bit_mask < max_bit_mask)
		{
			if (bit_mask & bit_order)
			{
				++bit_idx_cnt;
				if (bit_idx_cnt == idx)
				{
					result.push_back(bit_pos);
					bit_mask <<= 1;
					++bit_pos;
					break;
				}
			}
			bit_mask <<= 1;
			++bit_pos;
		}
		++cnt;
	}
	return result;
}

// Filter unsafe next states and get all safe next states
// If a state is previously visited and has shorter time, it will also be filtered out 
std::vector<state> get_safe_movs(const state& st, int boat_volume, const UsedStateTable& used_states)
{
	std::vector<state> results;
	std::vector<std::vector<int>> possible_combos;
	int grps[2];

	grps[0] = st.boat_side ? count_ones(st.mRight) : count_ones(st.mLeft);
	grps[1] = st.boat_side ? count_ones(st.cRight) : count_ones(st.cLeft);

	if (st.boat_side)	// Search strategy, if the boat is at the other side, choose fewer people first
	{
		for (int i = 1; i <= boat_volume; ++i)
		for (auto gc : get_multi_comb_idx_bits(grps, 2, i))
			possible_combos.push_back(std::move(gc));
	}
	else // Search strategy, try to take as many as possible people to the other side first
	{
		for (int i = boat_volume; i >= 1; --i)
		for (auto gc : get_multi_comb_idx_bits(grps, 2, i))
			possible_combos.push_back(std::move(gc));
	}

	for (auto pc : possible_combos)
	{
		unsigned int ml = st.mLeft;
		unsigned int cl = st.cLeft;
		unsigned int mr = st.mRight;
		unsigned int cr = st.cRight;

		unsigned int & m_from = st.boat_side ? mr : ml;
		unsigned int & m_to = st.boat_side ? ml : mr;
		unsigned int & c_from = st.boat_side ? cr : cl;
		unsigned int & c_to = st.boat_side ? cl : cr;

		int min_m_time = std::numeric_limits<int>::max();
		int min_c_time = std::numeric_limits<int>::max();

		std::vector<int> m_bit_index = std::move(bit_index_to_index(m_from, g_Missionary_N, pc[0]));
		std::vector<int> c_bit_index = std::move(bit_index_to_index(c_from, g_Cannible_N, pc[1]));

		if (!m_bit_index.empty())
			min_m_time = missionaries[m_bit_index[0]];	//because missionaries and m_bit_index are all sorted, so the first is the shortest in time
		if (!c_bit_index.empty())
			min_c_time = cannibals[c_bit_index[0]];

		for (int i : m_bit_index)	// move the missionaries from this state to next state
		{
			m_from &= (~(1<<i));
			m_to |= (1 << i);
		}
		for (int i : c_bit_index)	// move the cannibals from this state to next state
		{
			c_from &= (~(1 << i));
			c_to |= (1 << i);
		}
		// check if this next state is safe or not
		if (!is_safe_mov(count_ones(ml), count_ones(cl), count_ones(mr), count_ones(cr)))
			continue;

		bool found = false;
		unsigned long long key = ml;	// construct HashKey
		key <<= INTEGER_BITS;
		key |= ((cl << 1) | (st.boat_side ? 0 : 0x1));	// Note that boat_side is inverted here!
		auto iter = used_states.find(key);
		if (iter != used_states.end())
		{
			if (st.time + std::min(min_m_time, min_c_time) >= iter->second)
				found = true;
		}
		if (!found)
		{
			std::string mov(" ");
			for (int i : m_bit_index)
				mov = mov + 'm' + std::to_string(missionaries[i]);
			for (int i : c_bit_index)
				mov = mov + 'c' + std::to_string(cannibals[i]);

			results.emplace_back(ml, cl, mr, cr, !st.boat_side, st.movs + mov + (st.boat_side ? "<- " : "-> "), st.time + std::min(min_m_time, min_c_time));
		}
	}
	return results;
}

void find_solution_dfs(const state& cur_st, int boat_volume, UsedStateTable& used_states)
{
	if (solution_found(cur_st, boat_volume))
	{
		int min_m_time = std::numeric_limits<int>::max();
		int min_c_time = std::numeric_limits<int>::max();
		std::string s = cur_st.movs + " ";
		for (int i = 0; i < g_Missionary_N; ++i)
		{
			if (cur_st.mLeft & (1 << i))
			{
				int time = missionaries[i];
				s = s + 'm' + std::to_string(time);
				if (time < min_m_time)
					min_m_time = time;
			}
		}
		for (int i = 0; i < g_Cannible_N; ++i)
		{
			if (cur_st.cLeft & (1 << i))
			{
				int time = cannibals[i];
				s = s + 'c' + std::to_string(time);
				if (time < min_c_time)
					min_c_time = time;
			}
		}
		int cur_time = cur_st.time + std::min(min_m_time, min_c_time);
		if (cur_time < g_BestTime)
		{
			g_BestTime = cur_time;
			g_BestSolution = s + "->";
		}
	}
	else
	{
		bool found = false;
		unsigned long long key = cur_st.mLeft;
		key <<= INTEGER_BITS;
		key |= ((cur_st.cLeft << 1) | (cur_st.boat_side ? 0x1 : 0));
		auto iter = used_states.find(key);
		if(iter != used_states.end())
		{
			if (cur_st.time < iter->second)
				iter->second = cur_st.time;
			found = true;
		}
		if (!found)
			used_states.emplace(key, cur_st.time);
		for (auto gsm : get_safe_movs(cur_st, boat_volume, used_states))
			find_solution_dfs(gsm, boat_volume, used_states);
	}
}

void find_solution_bfs(std::queue<state>& cur_states, int boat_volume, UsedStateTable& used_states)
{
	while (!cur_states.empty())
	{
		state st = cur_states.front();
		cur_states.pop();
		bool found = false;
		unsigned long long key = st.mLeft;
		key <<= INTEGER_BITS;
		key |= ((st.cLeft << 1) | (st.boat_side ? 0x1 : 0));
		auto iter = used_states.find(key);
		if (iter != used_states.end())
		{
			if (st.time < iter->second)
				iter->second = st.time;
			found = true;
		}
		if (!found)
			used_states.emplace(key, st.time);

		for (auto gc : get_safe_movs(st, boat_volume, used_states))
		{
			if (gc.time >= g_BestTime)
				continue;
			if (solution_found(gc, boat_volume))
			{
				int min_m_time = std::numeric_limits<int>::max();
				int min_c_time = std::numeric_limits<int>::max();
				std::string s = gc.movs + " ";
				for (int i = 0; i < g_Missionary_N; ++i)
				{
					if (gc.mLeft & (1 << i))
					{
						int time = missionaries[i];
						s = s + 'm' + std::to_string(time);
						if (time < min_m_time)
							min_m_time = time;
					}
				}
				for (int i = 0; i < g_Cannible_N; ++i)
				{
					if (gc.cLeft & (1 << i))
					{
						int time = cannibals[i];
						s = s + 'c' + std::to_string(time);
						if (time < min_c_time)
							min_c_time = time;
					}
				}
				int cur_time = gc.time + std::min(min_m_time, min_c_time);
				if (cur_time < g_BestTime)
				{
					g_BestTime = cur_time;
					g_BestSolution = s + "->";
				}
			}
			else
				cur_states.push(std::move(gc));
		}
	}
}


int main(void)
{
	for (auto vi : get_all_combs_idx(5, 3))
	{
		for (auto i : vi)
			std::cout << i << ",  ";
		std::cout << std::endl;
	}
	int grp_cnts[] = { 3, 2, 2 };
	std::vector<std::vector<int>> multi_idxs_bits = get_multi_comb_idx_bits(grp_cnts, 3, 3);
	for (auto vi : multi_idxs_bits)
	{
		for (auto i : vi)
		{
			std::cout << "[ ";
			for (unsigned int idx = 0; idx<8 * sizeof(int); ++idx)
			if (i & (1 << idx))
				std::cout << idx << "  ";
			std::cout << "];  ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// Some initialization
	init_bit_cnt_table(BIT_CNT);
	g_Missionary_N = missionaries.size();
	g_Cannible_N = cannibals.size();

	std::cout << std::endl << "******************** DFS algorithm *********************" << std::endl << std::endl;
	UsedStateTable used_states;
	find_solution_dfs(state((1<<missionaries.size())-1, (1<<cannibals.size())-1, 0, 0, false, "", 0), boat_volume, used_states);

	if (g_BestTime != std::numeric_limits<int>::max())
		std::cout << "The best DFS time is: " << g_BestTime << ", DFS solution is: " << g_BestSolution << std::endl;
	else
		std::cout << "No solution" << std::endl;
/*
	std::cout << std::endl << "******************** BFS algorithm *********************" << std::endl << std::endl;
	g_BestTime = std::numeric_limits<int>::max();
	g_BestSolution = "";
	std::queue<state> q_states;
	used_states.clear();
	q_states.push(state((1<<missionaries.size())-1, (1<<cannibals.size())-1, 0, 0, false, "", 0));
	find_solution_bfs(q_states, boat_volume, used_states);

	if (g_BestTime != std::numeric_limits<int>::max())
		std::cout << "The best BFS time is: " << g_BestTime << ", BFS solution is: " << g_BestSolution << std::endl;
	else
		std::cout << "No solution" << std::endl;
*/
	return 0;
}
