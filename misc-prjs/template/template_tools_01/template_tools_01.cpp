#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <numeric>
#include <functional>

typedef unsigned long long UUL;

template<class Iter>
using IterValueType = typename std::iterator_traits<Iter>::value_type;

template<class T>
void do_print_container(const T& x, int& counter, int lvl, char prefix, char postfix, const char *l0_cont_delim)
{
	std::cout << x;
}
template<class T, template<class T, class ALLOC = std::allocator<T>> class CONTAINER>
void do_print_container(const CONTAINER<T>& cont, int& counter, int lvl, char prefix, char postfix, const char *l0_cont_delim)
{
	for (auto& x : cont)
	{
		std::cout << prefix;
		do_print_container(x, counter, lvl + 1, prefix, postfix, l0_cont_delim);
		std::cout << postfix;
		if (lvl == 0)
		{
			std::cout << l0_cont_delim;
			++counter;
		}
	}
}
// print contents of a container, return the number of elements in the container
template<class T>
int print_container(const T& cont, char prefix = '(', char postfix = ')', const char *l0_cont_delim = "  ")
{
	int elem_counter = 0;
	do_print_container(cont, elem_counter, 0, prefix, postfix, l0_cont_delim);
	std::cout << std::endl;
	return elem_counter;
}

/*
// sort vector of vector of integer types using string and in string-like order.
template<class IntType>
void sort_vec_vec_int_str(std::vector<std::vector<IntType>>& cont)
{
	std::sort(cont.begin(), cont.end(), [](const std::vector<IntType>& a, const std::vector<IntType>& b) \
	{
		return std::accumulate(a.begin(), a.end(), std::string(), [](const std::string& x, IntType y){ return x + std::string((char *)(&y), (char *)(&y) + sizeof(IntType)); }) \
			< std::accumulate(b.begin(), b.end(), std::string(), [](const std::string& x, IntType y){ return x + std::string((char *)(&y), (char *)(&y) + sizeof(IntType)); }); \
	});
}
*/
// sort vector of vector of integer types using string and in string-like order, using reinterpret_cast instead of C-style casting to make it more 'C++'.
template<class IntType>
void sort_vec_vec_int_str(std::vector<std::vector<IntType>>& cont)
{
	std::sort(cont.begin(), cont.end(), [](const std::vector<IntType>& a, const std::vector<IntType>& b) \
	{
		return std::accumulate(a.begin(), a.end(), std::string(), [](const std::string& x, IntType y){ return x + std::string(reinterpret_cast<char *>(&y), reinterpret_cast<char *>(&y) + sizeof(IntType)); }) \
			< std::accumulate(b.begin(), b.end(), std::string(), [](const std::string& x, IntType y){ return x + std::string(reinterpret_cast<char *>(&y), reinterpret_cast<char *>(&y) + sizeof(IntType)); }); \
	});
}


int main()
{
	std::string str("abcdef");
	std::vector<int> vi{ 1, 2, 3, 4, 5 };
	std::list<std::vector<int>> lvi{ { 9, 8 }, { 7, 6, 5 }, { 4 }, { 3, 2, 1 } };

	std::cout << "Total " << print_container(vi) << " elements in the container." << std::endl;
	std::cout << "Total " << print_container(lvi) << " elements in the container." << std::endl;
	std::cout << "Total " << print_container(str) << " elems in the container." << std::endl;

	std::vector<std::vector<int>> data{ { 1, 4, 5 }, { 1, 3, 4 }, { 1, 2, 3, 6 }, { 3, 1, 2 }, { 2, 1, 4 }, { 9, 7 } };
	//std::sort(data.begin(), data.end(), [&](const std::vector<int>& x, const std::vector<int>& y){ return std::accumulate(x.begin(), x.end(), std::string(), [](const std::string& a, int b){return a + std::string((char *)(&b), (char *)(&b) + sizeof(int)); }) < std::accumulate(y.begin(), y.end(), std::string(), [](const std::string& a, int b){return a + std::string((char *)(&b), (char *)(&b) + sizeof(int)); }); });
	sort_vec_vec_int_str(data);
	print_container(data);

	return 0;
}
