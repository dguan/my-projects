#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <chrono>

using namespace std;

int OneRound(vector<int>& left_bank, vector<int>& right_bank, int idx1, int idx2, string& solution)
{
	int x1 = left_bank[idx1];
	int x2 = left_bank[idx2];
	right_bank.push_back(x1);
	right_bank.push_back(x2);
	left_bank.erase(left_bank.begin() + idx1);
	left_bank.erase(std::find(left_bank.begin(), left_bank.end(), x2));
	solution += "(" + std::to_string(x1) + ", " + std::to_string(x2) + ")->  ";

	vector<int>::iterator itr = std::min_element(right_bank.begin(), right_bank.end());
	int t = *itr;
	left_bank.push_back(t);
	solution += "(" + std::to_string(t) + ")<-  ";
	right_bank.erase(itr);
	return t;
}

static int best_time = std::numeric_limits<int>::max();
static string best_solution;

int BridgeCrossing(const vector<int>& left_bank, const vector<int>& right_bank, int cur_time, string& solution)
{
	if (left_bank.size() == 2)
	{
		int tm = cur_time + std::max(left_bank[0], left_bank[1]);
		if (tm < best_time)
		{
			best_time = tm;
			best_solution = solution + "(" + std::to_string(left_bank[0]) + ", " + std::to_string(left_bank[1]) + ")->  ";
			return tm;
		}
		else return -1;
	}

	for (unsigned int i = 0; i < left_bank.size() - 1; ++i)
	{
		for (unsigned int j = i + 1; j < left_bank.size(); ++j)
		{
			int tm = cur_time + std::max(left_bank[i], left_bank[j]);
			if (tm >= best_time)
				continue;
			vector<int> cur_left_bank = left_bank;
			vector<int> cur_right_bank = right_bank;
			string cur_solution = solution;

			tm += OneRound(cur_left_bank, cur_right_bank, i, j, cur_solution);
			if (tm >= best_time)
				continue;

			int t = BridgeCrossing(cur_left_bank, cur_right_bank, tm, cur_solution);
			if (t < 0)
				continue;
			else
				tm += t;

			if (tm < best_time )
			{
				best_time = tm;
				best_solution = cur_solution;
			}
			continue;
		}
	}
	return best_time;
}


int main(void)
{
	int crossing_times[] = { 1, 2, 5, 10 };
	//int crossing_times[] = { 1, 2, 3, 4, 5 };
	//int crossing_times[] = { 1, 2, 3, 50, 99, 100 };
	//int crossing_times[] = { 2, 2, 3, 5, 5, 6, 7, 8, 10, 12 };

	vector<int> left_bank(std::begin(crossing_times), std::end(crossing_times));
	vector<int> right_bank;

	auto t0 = chrono::high_resolution_clock::now();
	BridgeCrossing(left_bank, right_bank, 0, string());
	auto t1 = chrono::high_resolution_clock::now();

	cout << "Best time is: " << best_time << endl;
	cout << "Best solution is: " << best_solution << endl;
	cout << "Computational time is: " << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() / 1000.0 << " seconds" << endl;
	return 0;
}

