//
// This problem is from StackOverflow,
// http://stackoverflow.com/questions/25349456/solving-subset-algorithm-using-a-recursive-way-gives-wrong-answer
//
// Problem Statement:
// Subsets Sum, is a classical problem in computer science, We are given a set of positive integer numbers;
// find out if there exists a non-empty subset of this set that the sum of its elements is equal to a given number.
//
// Example: suppose the set is[3, 4, 7, 9, 10] and the target number is 20, there exists a subset[3, 7, 10],
// the sum of this subset is equal to 20.
//
// Jarod42 gave out an excellent solution, my first thought has been recursion, and gave out a recursive solution,
// later I thought out an improved recursive solution which not only find out if subset exist or not, but also
// print all subsets if such subset exist.
//

/*
//
// Jarod42's solution:
//
bool SS(const std::vector<int>& v, int total)
{
	std::set<int> sums;
	for (auto e : v) {
		std::set<int> r = sums;
		r.insert(e);
		for (auto s : sums) {
			r.insert(s + e);
		}
		sums.swap(r);
	}
	return sums.count(total);
}
*/

#include <iostream>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Why this recursive algorithm subset_exist is correct:
//
// Suppose we have a dataset {X1, X2, ..., Xn}, and the target value is V,
// if there exists at least one subset, then either there's one Xi whose value is V,
// or Xi be part of a subset, so there must be a subset in {X1, ...Xi-1, Xi+1, Xn} whose
// target value is V - Xi. So, from above induction, this algorithm is correct.
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool subset_exist(const std::vector<int>& input_data, int target_value)
{
	if (input_data.size() == 1)
		return input_data[0] == target_value;

	bool result = false;
	for (unsigned int i = 0; i < input_data.size(); ++i)
	{
		std::vector<int> copy = input_data;
		copy.erase(copy.begin() + i);
		if (input_data[i] == target_value || subset_exist(copy, target_value - input_data[i]))
		{
			result = true;
			break;
		}
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// The correctness of this find_subsets is the same as above subset_exist, no need to repeat.
//
// One thing to be pointed out that, because I did a exhaustive recursive search, if we found
// a single value that eaquals the target value, that is simple, that value get saved as a
// solution, but if we found a subset of N number's combination, because of the exhaustive search,
// each permutation of that combination will be counted as a solution. Like in the testcase_3,
// it will find 7 solutions, (7, 5, 1), (5, 7, 1), (7, 1, 5), (1, 7, 5), (5, 1, 7), (1, 5, 7),
// and (13), the things will get even more complicated if there exists duplications in the dataset.
//
// One solution is to setup a set to get unique solutions, when a subset is found, get the subset
// sorted first, then we put that solution into set, so all solutions are unique. Using a Hash table
// also works, using sorted subset as hash key, check if the solution already exists in the Hash table,
// if it is not there then add it, otherwise discard it.
//
// Another thing to be pointed out is that, this algorithm is not scalable, the complexity increase
// in exponential scale. Using set or Hash table can prove this to certain extent, yet it is still complex.
//
// Because there's already the StackOverflow solution, I am not going to give the non-recursive solution here, 
//
/////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::vector<int>> find_subsets(const std::vector<int>& input, int target)
{
	std::vector<std::vector<int>> results;
	if (input.size() == 1)
	{
		if (input[0] == target)
			results.emplace_back(1, input[0]);
		return results;
	}

	for (unsigned int i = 0; i < input.size(); ++i)
	{
		std::vector<int> copy = input;
		copy.erase(copy.begin() + i);
		if (input[i] == target)
			results.emplace_back(1, input[i]);
		else
		{
			for (auto x : find_subsets(copy, target - input[i]))
			{
				x.push_back(input[i]);
				results.push_back(std::move(x));
			}
		}
	}
	return results;
}

int main()
{
	std::vector<int> testcase_1{45, 26, 36, 4, 8};
	int target_1 = 1;
	std::vector<int> testcase_2{49, 9, 5, 37, 0, 42, 15, 19};
	int target_2 = 49;
	std::vector<int> testcase_3{ 1, 3, 5, 7, 11, 13 };
	int target_3 = 13; 
	std::vector<int> testcase_4{ 1, 3, 5, 7 };
	int target_4 = 14;

	std::cout << (subset_exist(testcase_1, target_1) ? "Yes" : "No") << std::endl;
	std::cout << (subset_exist(testcase_2, target_2) ? "Yes" : "No") << std::endl;
	std::cout << (subset_exist(testcase_3, target_3) ? "Yes" : "No") << std::endl;
	std::cout << (subset_exist(testcase_4, target_4) ? "Yes" : "No") << std::endl;

	std::cout << "************************************************************" << std::endl;

	int count = 0;
	for (auto sub : find_subsets(testcase_2, target_2))
	{
		for (int i : sub)
			std::cout << i << ", ";
		std::cout << std::endl;
		++count;
	}
	if (count > 0)
		std::cout << "Altogether " << count << " subsets found." << std::endl;
	else
		std::cout << "No solution." << std::endl;

	std::cout << "************************************************************" << std::endl;

	count = 0;
	for (auto sub : find_subsets(testcase_3, target_3))
	{
		for (int i : sub)
			std::cout << i << ", ";
		std::cout << std::endl;
		++count;
	}
	if (count > 0)
		std::cout << "Altogether " << count << " subsets found." << std::endl;
	else
		std::cout << "No solution." << std::endl;

	return 0;
}
