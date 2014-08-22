#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
#include <utility>
#include <exception>
#include <stdexcept>


// Get the n-th order binomial coefficient using Dynamic Programming
template<class RetIntType=unsigned int>
std::vector<RetIntType> binomial_coefficient(int n)
{
	std::vector<RetIntType> result(n + 1, 1);
	for (int i = 1; i < n; ++i)
	{
		RetIntType prev = 1;
		for (int j = 1; j <= i; ++j)
		{
			std::swap(prev, result[j]);
			result[j] += prev;
		}
	}
	return result;
}


int main(void)
{
	for(int i=0; i<10; i++)
	{
		for(auto coeff : binomial_coefficient(i))
			std::cout << coeff << "\t";
		std::cout << std::endl;
	}

	return 0;
}


