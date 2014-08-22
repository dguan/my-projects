#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

template <typename Iter>
using IterValueType = typename std::iterator_traits<Iter>::value_type;


////////////////// recursively print contents of a container ///////////////////
template<class T>
void print_container(const T& x)
{
	std::cout << x;
}
template<class T, template<class T, class ALLOC = std::allocator<T>> class CONT>
void print_container(const CONT<T>& cont)
{
	for (auto x : cont)
	{
		std::cout << "(";
		print_container(x);
		std::cout << ")";
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////  partition length of a string or sequences ////////////////
//////////////////////////////////////////////////////////////////////////

// This function generates all possible partitions of a sequence in each partition's length
std::vector<std::vector<int>> part_len(int N)
{
	std::vector<std::vector<int>> parts;
	if (N == 0)
	{
		parts.push_back(std::vector<int>());
		return parts;
	}
	else if (N == 1)
	{
		parts.emplace_back(1, 1);
		return parts;
	}
	else
	{
		for (int i = 1; i <= N; ++i)
		{
			for (auto vi : part_len(N - i))
			{
				vi.insert(vi.begin(), i);
				parts.push_back(std::move(vi));
			}
		}
	}
	return parts;
}

// Partition a sequence of length N into sub-partitions whose length not longer than max_part_len;
// return partitions in each partition's length
std::vector<std::vector<int>> part_len_with_len_cap(int seq_len, int max_part_len)
{
	std::vector<std::vector<int>> parts;
	if (seq_len == 0)
	{
		parts.push_back(std::vector<int>());
		return parts;
	}
	else if (seq_len == 1)
	{
		parts.emplace_back(1, 1);
		return parts;
	}
	else
	{
		for (int i = 1; i <= max_part_len; ++i)
		{
			for (auto vi : part_len_with_len_cap(seq_len - i, std::min<int>(max_part_len, seq_len - i)))
			{
				vi.insert(vi.begin(), i);
				parts.push_back(std::move(vi));
			}
		}
	}
	return parts;
}

/*
// Partition a sequence into partitions whose length are between min_part_len and max_part_len
// Make sure before calling that seq_len >= max_part_len >= min_part_len >= 1
std::vector<std::vector<int>> part_len_with_len_range(int seq_len, int min_part_len, int max_part_len)
{
	std::vector<std::vector<int>> parts;
	if (seq_len == 0)
	{
		parts.push_back(std::vector<int>());
		return parts;
	}
	else if (seq_len == min_part_len)
	{
		parts.emplace_back(1, min_part_len);
		return parts;
	}
	else
	{
		for (int i = min_part_len; i <= max_part_len; ++i)
		{
			for (auto vi : part_len_with_len_range(seq_len - i, min_part_len, std::min<int>(max_part_len, seq_len - i)))
			{
				vi.insert(vi.begin(), i);
				parts.push_back(std::move(vi));
			}
		}
	}
	return parts;
}
*/

// Partition a sequence into partitions whose length are between min_part_len and max_part_len
// Make sure before calling that seq_len >= max_part_len >= min_part_len >= 1
// This generic templated function replaces the above seq_part_len_range
template<class IntType=int, template<class IntType, class ALLOC = std::allocator<IntType>> class outer_container = std::vector, template<class IntType, class ALLOC = std::allocator<IntType>> class inner_container = std::vector>
outer_container<inner_container<IntType>> part_len_with_len_range(IntType seq_len, int min_len, int max_len)
{
	outer_container<inner_container<IntType>> parts;
	if (seq_len == 0)
	{
		parts.push_back(inner_container<IntType>());
		return parts;
	}
	else if (seq_len == min_len)
	{
		parts.emplace_back(1, min_len);
		return parts;
	}
	else
	{
		for (int i = min_len; i <= max_len; ++i)
		{
			for (auto in_cont : part_len_with_len_range(seq_len - i, min_len, std::min<int>(max_len, seq_len - i)))
			{
				in_cont.insert(in_cont.begin(), i);
				parts.push_back(std::move(in_cont));
			}
		}
	}
	return parts;
}

// partition seq_len into num_parts partitions
std::vector<std::vector<int>> part_len_into_parts(int seq_len, int num_parts)
{
	std::vector<std::vector<int>> results;
	if (num_parts == 1)
	{
		results.emplace_back(1, seq_len);
		return results;
	}
	for (int i = 1; i <= seq_len - num_parts + 1; ++i)
	{
		for (auto vi : part_len_into_parts(seq_len - i, num_parts - 1))
		{
			vi.insert(vi.begin(), i);
			results.push_back(std::move(vi));
		}
	}
	return results;
}


//////////////////////////////////////////////////////////////////////////
/////////////////////////  partition a string ////////////////////////////
//////////////////////////////////////////////////////////////////////////

// partition a string
template<class StrType>
std::vector<std::vector<StrType>> part_str(const StrType& str)
{
	std::vector<std::vector<StrType>> parts;
	int str_len = str.length();
	if (str_len == 0)
	{
		parts.push_back(std::vector<StrType>());
		return parts;
	}
	else if (str_len == 1)
	{
		parts.emplace_back(1, str);
		return parts;
	}
	else
	{
		for (int i = 1; i <= str_len; ++i)
		{
			StrType prefix = str.substr(0, i);
			for (auto vec_str : part_str(str.substr(i)))
			{
				vec_str.insert(vec_str.begin(), prefix);
				parts.push_back(std::move(vec_str));
			}
		}
	}
	return parts;
}

// partition a string into substrings whose length are between min_str_len and max_str_len
template<class StrType>
std::vector<std::vector<StrType>> part_str_with_len_range(const StrType& str, int min_part_len, int max_part_len)
{
	std::vector<std::vector<StrType>> parts;
	int str_len = str.length();
	if (str_len == 0)
	{
		parts.push_back(std::vector<StrType>());
		return parts;
	}
	else if (str_len == min_part_len)
	{
		parts.emplace_back(1, str);
		return parts;
	}
	else
	{
		for (int i = min_part_len; i <= max_part_len; ++i)
		{
			StrType pre = str.substr(0, i);
			for (auto in_cont : part_str_with_len_range(str.substr(i), min_part_len, std::min<int>(max_part_len, str_len - i)))
			{
				in_cont.insert(in_cont.begin(), pre);
				parts.push_back(std::move(in_cont));
			}
		}
	}
	return parts;
}

// partition a string into parts
template<class StrType>
std::vector<std::vector<StrType>> part_str_into_parts(const StrType& str, int num_parts)
{
	std::vector<std::vector<StrType>> parts;
	int str_len = str.length();

	if (num_parts == 1)
	{
		parts.emplace_back(1, str);
		return parts;
	}

	for (int i = 1; i <= str_len - num_parts + 1; ++i)
	{
		StrType pre = str.substr(0, i);
		for (auto vs : part_str_into_parts(str.substr(i), num_parts-1))
		{
			vs.insert(vs.begin(), pre);
			parts.push_back(std::move(vs));
		}
	}
	return parts;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////// partition a sequences ///////////////////////////
//////////////////////////////////////////////////////////////////////////

// partition a sequence inside a container into sub-sequences whose length between min_part_len and max_part_len
template<class T, template<class T, class AllOC = std::allocator<T>> class DataContainer, template<class T, class ALLOC = std::allocator<T>> class outer_container = std::vector, template<class T, class ALLOC = std::allocator<T>> class inner_container = std::vector>
outer_container<inner_container<DataContainer<T>>> part_seq_with_len_range(const DataContainer<T>& seq, int min_part_len, int max_part_len)
{
	outer_container<inner_container<DataContainer<T>>> parts;
	int seq_len = seq.size();
	if (seq_len == 0)
	{
		parts.push_back(inner_container<DataContainer<T>>());
		return parts;
	}
	else if (seq_len == min_part_len)
	{
		parts.emplace_back(inner_container<DataContainer<T>>(1, seq));
		return parts;
	}
	else
	{
		for (int i = min_part_len; i <= max_part_len; ++i)
		{
			DataContainer<T> pre(seq.begin(), std::next(seq.begin(), i));
			DataContainer<T> remain(std::next(seq.begin(), i), seq.end());
			for (auto in_cont : part_seq_with_len_range(remain, min_part_len, std::min<int>(max_part_len, seq_len - i)))
			{
				in_cont.insert(in_cont.begin(), pre);
				parts.push_back(std::move(in_cont));
			}
		}
	}
	return parts;
}

// partition a sequence inside a container into sub-sequences whose length between min_part_len and max_part_len
template<class ForwardIter>
std::vector<std::vector<std::vector<IterValueType<ForwardIter>>>> //std::vector<std::vector<std::vector<typename std::iterator_traits<ForwardIter>::value_type>>>
part_seq_with_len_range(const ForwardIter seq_begin, const ForwardIter seq_end, int min_part_len, int max_part_len)
{
	typedef IterValueType<ForwardIter> ValueType;
	std::vector<std::vector<std::vector<ValueType>>> parts;
	int seq_len = std::distance(seq_begin, seq_end);
	if (seq_len == 0)
	{
		parts.push_back(std::vector<std::vector<ValueType>>());
		return parts;
	}
	else if (seq_len == min_part_len)
	{
		parts.emplace_back(1, std::vector<ValueType>(seq_begin, seq_end));
		return parts;
	}
	else
	{
		for (int i = min_part_len; i <= max_part_len; ++i)
		{
			std::vector<ValueType> pre(seq_begin, std::next(seq_begin, i));
			for (auto vvt : part_seq_with_len_range(std::next(seq_begin, i), seq_end, min_part_len, std::min<int>(max_part_len, seq_len - i)))
			{
				vvt.insert(vvt.begin(), pre);
				parts.push_back(std::move(vvt));
			}
		}
	}
	return parts;
}

// partition a sequence inside a container into sub-sequences whose length between min_part_len and max_part_len
template<class ForwardIter>
std::vector<std::vector<std::vector<IterValueType<ForwardIter>>>> //std::vector<std::vector<std::vector<typename std::iterator_traits<ForwardIter>::value_type>>>
part_seq_into_parts(const ForwardIter seq_begin, const ForwardIter seq_end, int num_parts)
{
	typedef IterValueType<ForwardIter> ValueType;
	std::vector<std::vector<std::vector<ValueType>>> parts;
	int seq_len = std::distance(seq_begin, seq_end);

	if (num_parts == 1)
	{
		parts.emplace_back(1, std::vector<ValueType>(seq_begin, seq_end));
		return parts;
	}

	for (int i = 1; i <= seq_len - num_parts + 1; ++i)
	{
		std::vector<ValueType> pre(seq_begin, std::next(seq_begin, i));
		for (auto vvt : part_seq_into_parts(std::next(seq_begin, i), seq_end, num_parts - 1))
		{
			vvt.insert(vvt.begin(), pre);
			parts.push_back(std::move(vvt));
		}
	}
	return parts;
}


int main()
{
	///////////////// partition length of string or sequence //////////////////
	int seq_len = 5;
	std::cout << "partition a sequence of length " << seq_len << std::endl;
	int cnt = 0;
	for (auto vi : part_len(seq_len))
	{
		for (auto i : vi)
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt << " partitions" << std::endl;

	seq_len = 5;
	int part_len_cap = 2;
	cnt = 0;
	std::cout << "partition a sequence of length " << seq_len << " with part_len cap set to " << part_len_cap << std::endl;
	for (auto vi : part_len_with_len_cap(seq_len, part_len_cap))
	{
		for (auto i : vi)
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt << " partitions" << std::endl;

	seq_len = 7;
	int min_part_len = 2;
	int max_part_len = 3;
	cnt = 0;
	std::cout << "partition a sequence of length " << seq_len << " with min and max part_len set to " << min_part_len << " and " << max_part_len << std::endl;
	for (auto vi : part_len_with_len_range(seq_len, min_part_len, max_part_len))
	{
		for (auto i : vi)
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt << " partitions" << std::endl;

	seq_len = 7;
	int num_parts = 2;
	cnt = 0;
	std::cout << "partition a sequence of length " << seq_len << " into " << num_parts << " parts" << std::endl;
	for (auto vi : part_len_into_parts(seq_len, num_parts))
	{
		for (auto i : vi)
			std::cout << i << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt << " partitions" << std::endl;

///////////////// partition  strings  //////////////////

	cnt = 0;
	std::string str("abcde");
	std::cout << "partition a string : " << str << std::endl;
	for (auto& vs : part_str(str))
	{
		for (auto& s : vs)
			std::cout << s << ", ";
		std::cout << std::endl;
		++cnt;
	}
	std::cout << "Altogether " << cnt << " partitions" << std::endl;

	min_part_len = 1;
	max_part_len = 2;
	std::cout << "partition a string : " << str << " with min and max part_len set to " << min_part_len << " and " << max_part_len << std::endl;
	for (auto x : part_str_with_len_range(str, min_part_len, max_part_len))
	{
		print_container(x);
		std::cout << std::endl;
	}

	num_parts = 3;
	std::cout << "partition a string : " << str << " into " << num_parts << " parts" << std::endl;
	for (auto x : part_str_into_parts(str, num_parts))
	{
		print_container(x);
		std::cout << std::endl;
	}

	///////////////// partition  sequences  //////////////////

	std::vector<int> vi{ 1, 2, 3, 4, 5 };

	min_part_len = 1;
	max_part_len = 2;
	std::cout << "partition a sequence : ";
	print_container(vi);
	std::cout << " with min and max part_len set to " << min_part_len << " and " << max_part_len << std::endl;
	for (auto x : part_seq_with_len_range(vi, 1, 2))
	{
		print_container(x);
		std::cout << std::endl;
	}
	std::cout << "partition a sequence : ";
	print_container(vi);
	std::cout << " with min and max part_len set to " << min_part_len << " and " << max_part_len << " using iterators" << std::endl;
	for (auto x : part_seq_with_len_range(vi.begin(), vi.end(), 1, 2))
	{
		print_container(x);
		std::cout << std::endl;
	}

	num_parts = 3;
	std::cout << "partition a sequence : ";
	print_container(vi);
	std::cout << " into " << num_parts << " parts" << std::endl;
	for (auto x : part_seq_into_parts(vi.begin(), vi.end(), num_parts))
	{
		print_container(x);
		std::cout << std::endl;
	}

	return 0;
}

