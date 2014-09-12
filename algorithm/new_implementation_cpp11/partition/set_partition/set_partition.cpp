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
int print_container(const T& cont, char prefix = '(', char postfix = ')', const char *l0_cont_delim = "\n")
{
	int elem_counter = 0;
	do_print_container(cont, elem_counter, 0, prefix, postfix, l0_cont_delim);
	std::cout << std::endl;
	return elem_counter;
}



// recursive set partitioning algorithm based on index
std::vector<std::vector<std::vector<int>>> set_partition_idx(int N)
{
	std::vector<std::vector<std::vector<int>>> parts;
	if (N == 1)
	{
		std::vector<std::vector<int>> temp;
		temp.emplace_back(1, 0);
		parts.push_back(temp);
		return parts;
	}
	for (auto vvi : set_partition_idx(N-1))
	{
		for (unsigned int i = 0; i < vvi.size(); ++i)
		{
			std::vector<std::vector<int>> temp = vvi;
			temp[i].push_back(N - 1);
			parts.push_back(std::move(temp));
		}
		vvi.emplace_back(1, N - 1);
		parts.push_back(std::move(vvi));
	}
	return parts;
}

// using one bit to represent an index,
// so, N must <= total bits in the integer type
// 32-bit integer should be enough for most situations
// if that's not enough, well try 64-bit integer
std::vector<std::vector<int>> set_partition_bit(int N)
{
	std::vector<std::vector<int>> parts;
	if (N == 1)
	{
		parts.emplace_back(1, 1);
		return parts;
	}
	for (auto vi : set_partition_bit(N - 1))
	{
		for (unsigned int i = 0; i < vi.size(); ++i)
		{
			std::vector<int> temp = vi;
			temp[i] |= (1 << (N - 1));
			parts.push_back(std::move(temp));
		}
		vi.push_back(1 << (N - 1));
		parts.push_back(std::move(vi));
	}
	return parts;
}


// I have not found a way to implement the next_partition or prev_partition method,
// and I have not found from web any clue of doing this. Yet, from various combinatonic
// or partitioning algorithms I studied and implemented, I believe there should be one
// such ordering for set partition, and the algorithm based on this ordering. This may
// need some time to find this ordering and algorithm.


int main()
{
	std::cout << "Total " << print_container(set_partition_idx(4)) << " partitions." << std::endl << std::endl;
	std::cout << "Total " << print_container(set_partition_bit(4)) << " partitions." << std::endl << std::endl;

	std::cout << "Total " << print_container(set_partition_idx(5)) << " partitions." << std::endl << std::endl;
	std::cout << "Total " << print_container(set_partition_bit(5)) << " partitions." << std::endl << std::endl;

	return 0;
}
