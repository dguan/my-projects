#include <iostream>
#include <vector>

template<class T>
void do_print_container(const T& x, int& counter, int lvl, char prefix, char postfix, const char *l0_cont_delim)
{
	std::cout << x;
}
template<class T, template<class T, class ALLOC = std::allocator<T>> class CONTAINER>
void do_print_container(const CONTAINER<T>& cont, int& counter, int lvl, char prefix, char postfix, const char *l0_cont_delim)
{
	for (auto x : cont)
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
template<class T>
int print_container(const T& cont, char prefix = '(', char postfix = ')', const char *l0_cont_delim = "  ")
{
	int elem_counter = 0;
	do_print_container(cont, elem_counter, 0, prefix, postfix, l0_cont_delim);
	std::cout << std::endl;
	return elem_counter;
}

std::vector<unsigned long long> bell_number_serie(int N)
{
	std::vector<unsigned long long> bell_serie(N + 1);
	std::vector<unsigned long long> aux_vector(N + 1);
	bell_serie[0] = 1;
	for (int i = 1; i <= N; ++i)
	{
		unsigned long long temp = aux_vector[1];
		aux_vector[1] = bell_serie[i - 1];
		for (int j = 2; j <= i; ++j)
		{
			temp += aux_vector[j - 1];
			std::swap(aux_vector[j], temp);
		}
		bell_serie[i] = aux_vector[i];
	}
	return bell_serie;
}

int main()
{
	print_container(bell_number_serie(20));

	return 0;
}

