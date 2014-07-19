#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//#include <functional>

// make sure before calling that a <= b
//int find_gcd(int a, int b)
//{
//	return a == 0 ? b : find_gcd(b%a, a);
//}

// make sure before calling that a <= b, and a>=0
template<class T> T find_gcd(T a, T b)
{
	while (a != 0)
	{
		T x = b%a;
		b = a;
		a = x;
	}
	return b;
}

std::vector<std::vector<int> > egyptian_number(int Max, int n, int start, unsigned long up, unsigned long low)
{
	std::vector<std::vector<int> >results;
	if (n == 1)
	{
		if (low%up == 0 && low / up >= start)
			results.emplace_back(1, low / up);
		return results;
	}
		
	int end_point = (unsigned long)low*n / up + 1;
	if (end_point > Max)
		end_point = Max;
	//std::cout<<"start = "<<start<<", end_point = "<<end_point<<std::endl;

	for (int i = start; i<end_point; ++i)
	{
		if (i*up <= low)
			continue;
		unsigned long new_up = i*up - low;
		unsigned long new_low = i*low;

		//must > 1/Max
		if (Max*new_up <= new_low)
			continue;

		 auto gcd = find_gcd(new_up, new_low);

		for (auto vi : egyptian_number(Max, n - 1, i + 1, new_up/gcd, new_low/gcd))
		{
			vi.push_back(i);
			results.push_back(std::move(vi));
		}
	}
	return results;
}

int main(void)
{
	while (1)
	{
		//the total results increases terribly as max_number, and, especially num_parts, increase.
		//this is a torlarable settings, and will give out 9855 results.
		//int max_number = 128;
		//int num_parts = 8;
		int max_number, num_parts;
		std::cout << "Please input the maximum number and how many parts seperated with a space." << std::endl;
		std::cout << "input 0 or negative number to quit." << std::endl;
		std::cout << "Note that, it is not recommended to set parts bigger than 8," << std::endl;
		std::cout << "Max number and parts are: ";
		std::cin >> max_number >> num_parts;
		if (max_number <= 0 || num_parts <= 0)
			break;

		int num_results = 0;
		for (auto vi : egyptian_number(max_number, num_parts, 2, 1, 1))
		{
			std::reverse(vi.begin(), vi.end());
			for (auto i : vi)
				std::cout << i << ", ";
			std::cout << std::endl;
			++num_results;
		}
		std::cout << "---------------------------------------------------" << std::endl;
		std::cout << "There're altogether " << num_results << " solutions found." << std::endl;
	}
	return 0;
}

