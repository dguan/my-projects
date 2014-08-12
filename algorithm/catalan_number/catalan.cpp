#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <bitset>
#include <iterator>


// Actually there can be many more ways to generate Catalan number, for example using combination formula, etc.
// If you study wikipedia carefully, you can easily find more ways to get the Catalan number, I only used a few
// most important (or most useful) methods.  

//
// Two compile time template Catalan number generators
//

// To check if M greater than N
template<int M, int N> struct M_GT_N
{
	enum{ val = M > N };
};

// Calculate Catalan Number
template<int M, int N, bool flag> struct CATALAN
{
	enum class v : int64_t { val = static_cast<int64_t>(CATALAN<M - 1, N, M_GT_N<M - 1, N>::val>::v::val) + static_cast<int64_t>(CATALAN<M, N - 1, M_GT_N<M, N - 1>::val>::v::val) };
};
template<int M, int N> struct CATALAN<M, N, true>
{
	enum class v : int64_t { val = 0 };
};
template<int N> struct CATALAN<0, N, false>
{
	enum class v : int64_t { val = 1 };
};

// Another way to calculate Catalan Number
template<int M, int I> struct CAT_SUM;
template<int M> struct OTHER_CATALAN
{
	enum class v : int64_t { val = static_cast<int64_t>(CAT_SUM<M - 1, M - 1>::v::val) };
};
template<> struct OTHER_CATALAN<0>
{
	enum class v : int64_t { val = 1 };
};
template<int M, int I> struct CAT_SUM
{
	enum class v : int64_t { val = static_cast<int64_t>(OTHER_CATALAN<M - I>::v::val) * static_cast<int64_t>(OTHER_CATALAN<I>::v::val) + static_cast<int64_t>(CAT_SUM<M, I - 1>::v::val) };
};
template<int M> struct CAT_SUM<M, -1>
{
	enum class v : int64_t { val = 0 };
};


//
// Two recursive and one DP Catalan number generators
//

unsigned long catalan_number_recursive_1(int N)
{
	unsigned long result = 0;
	if (N <= 1)
		return 1;
	for (int i = 0; i < N; ++i)
		result += catalan_number_recursive_1(i) * catalan_number_recursive_1(N - 1 - i);
	return result;
}

unsigned long catalan_number_recursive_2(int M, int N)
{
	if (M == 0)
		return 1;
	else if (M == N)
		return catalan_number_recursive_2(M - 1, N);
	else if (M < N)
		return catalan_number_recursive_2(M - 1, N) + catalan_number_recursive_2(M, N - 1);
}

unsigned long catalan_number_dp(int N)
{
	std::vector<unsigned long> dp_arr(N + 1, 0);
	dp_arr[0] = 1;
	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= i; ++j)
			dp_arr[j] = dp_arr[j - 1] + dp_arr[j];
	return dp_arr[N];
}

//
// Two DP Catalan Number sequence generators
//

std::vector<unsigned long> catalan_seq_dp_1(int N)
{
	std::vector<unsigned long> results;
	std::vector<std::vector<unsigned long>> dp_arr(N + 1, std::vector<unsigned long>(N + 1, 0));
	for (int i = 0; i <= N; ++i)
		dp_arr[0][i] = 1;
	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= i; ++j)
			dp_arr[i][j] = dp_arr[i][j - 1] + dp_arr[i - 1][j];
	for (int i = 1; i <= N; ++i)
		results.push_back(dp_arr[i][i]);
	return results;
}

std::vector<unsigned long> catalan_seq_dp_2(int N)
{
	std::vector<unsigned long> dp_arr(N, 0);
	dp_arr[0] = 1;
	for (int i = 1; i < N; ++i)
	{
		int j = i;
		while (--j >= 0)
			dp_arr[i] += dp_arr[i - 1 - j] * dp_arr[j];
	}
	return dp_arr;
}

//
// Two recursive Catalan string generators
//

// Of course you could use any char sets like '(' and ')' to replace '1' and '0'
std::vector<std::string> catalan_string_1(int N)
{
	std::vector<std::string> results;
	if (N == 0)
	{
		results.emplace_back("");
		return results;
	}
	int i = N;
	while (--i >= 0)
		for (auto s1 : catalan_string_1(i))
			for (auto s2 : catalan_string_1(N - 1 - i))
				results.emplace_back('1' + s1 + '0' + s2);	//results.emplace_back(s1 + '1' + s2 + '0');
	return results;
}


void catalan_string_2(int M, int N, const std::string& cur_seq, std::vector<std::string>& results)
{
	if (M==0)
		results.emplace_back(cur_seq + std::string(N, ')'));
	else if (M == N)
		catalan_string_2(M - 1, N, cur_seq + '(', results);
	else if (M < N)
	{
		catalan_string_2(M - 1, N, cur_seq + '(', results);
		catalan_string_2(M, N - 1, cur_seq + ')', results);
	}
}

//
// Two recursive Catalan integer(bit sequence) generators
//
// Ideally, it would be nice to use dynamic_bitsets to implement these two generators, but currently,
// C++11 does not have dynamic_bitset(I cannot believe it that they didn't put this into C++11, what the f*ck?),
// although Boost have a dynamic_bitset, but I would like not to introduce third party library into pure C++ codes.
//

// For an 32-bit unsigned long, obviously, you cannot make N > 16
// if you need bigger Catalan number, you might want to use catalan_string instead.
std::vector<unsigned long> catalan_integer_1(int N)
{
	std::vector<unsigned long> results;
	if (N == 0)
	{
		results.push_back(0UL);
		return results;
	}
	int i = N;
	while (--i >= 0)
		for (auto x : catalan_integer_1(i))
			for (auto y : catalan_integer_1(N - 1 - i))
				//results.push_back((((x<<1) | (1<<(i*2+1))) << (2*(N-1-i))) | y);
				results.push_back((x << 1) | (1 << (i * 2 + 1)) | (y << (2 * i + 2)));	// These 2 lines are equivalent
	return results;
}

//
// Be aware of the order of the last 2 recursive calls, here execution order matters.
// In this order, the results are in lexigraphical order, if you swap the order
// of these two recursive calls, the order of the results will change.
//
void catalan_integer_2(int M, int N, unsigned long cur_int, std::vector<unsigned long>& results)
{
	if (M == 0)
		results.push_back(cur_int << N);
	else if (M == N)
		catalan_integer_2(M - 1, N, cur_int << 1 | 0x1, results);
	else if (M < N)
	{
		//catalan_integer_2(M, N - 1, cur_int << 1, results);
		catalan_integer_2(M - 1, N, cur_int << 1 | 0x1, results);
		catalan_integer_2(M, N - 1, cur_int << 1, results);
	}
}

// Check if a char sequence is a valid Catalan sequence
template<class ConstBiIter, class char_type=char>
bool is_catalan_seq(ConstBiIter seq_begin, ConstBiIter seq_end, char_type char_1st='1', char_type char_2nd='0')
{
	int seq_len = std::distance(seq_begin, seq_end);
	if (seq_len % 2)
		return false;
	ConstBiIter last = std::prev(seq_end);
	if (*seq_begin != char_1st || *last != char_2nd)
		return false;
	int first_char_cnt = 0;
	int second_char_cnt = 1;
	while (--last != seq_begin)
	{
		if (*last == char_1st)
			++first_char_cnt;
		else if (*last == char_2nd)
			++second_char_cnt;
		else
			return false;
		if (first_char_cnt > second_char_cnt)
			return false;
	}
	return true;
}

// Check if a bit sequence is a valid Catalan bit sequence, for simplicity, and as C++11 does not have dynamic bitset, I just used integer type as bit sequence
template<class int_type = unsigned int>
bool is_catalan_int(int_type x, int bits_len)
{
	if (bits_len % 2)
		return false;
	unsigned int bit_mask = 0x1;
	if (x & bit_mask || !(x & (bit_mask<<(bits_len-1))))
		return false;
	int one_cnt = 0;
	int zero_cnt = 0;
	for(int i=0; i<bits_len; ++i)
	{
		if (x & bit_mask)
			++one_cnt;
		else
			++zero_cnt;
		if (one_cnt > zero_cnt)
			return false;
		else
			bit_mask <<= 1;
	}
	return true;
}

// Generate next Catalan integer, say N=4, the first Catalan integer is 0xF0, the last one is 0xAA
template<class unsigned_int_type>
bool next_catalan_int(unsigned_int_type& x, int bits_len)
{
	unsigned int bit_mask = 0x2;
	unsigned int max_bit_mask = 1 << bits_len;
	int one_cnt = 0;
	while (bit_mask < max_bit_mask)
	{
		if (x & bit_mask)
		{
			if (bit_mask >= 4 && !(x&(bit_mask >> 2)))
				break;
			else
				++one_cnt;
		}
		bit_mask <<= 1;
	}
	if (bit_mask >= max_bit_mask)
		return false;
	x ^= bit_mask;
	while (one_cnt-- >= 0)
	{
		bit_mask >>= 1;
		x |= bit_mask;
	}
	x &= ~(bit_mask-1);
	return true;
}

// Generate previous Catalan integer, say N=4, the first Catalan integer is 0xAA, the last one is 0xF0
template<class unsigned_int_type>
bool prev_catalan_int(unsigned_int_type& x, int bits_len)
{
	unsigned int bit_mask = 0x2;
	unsigned int max_bit_mask = 1 << bits_len;
	int one_cnt = 0;
	while (!(x & bit_mask))
		bit_mask <<= 1;
	if (bit_mask >= 1 << (bits_len>>1))
		return false;
	++one_cnt;
	bit_mask <<= 1;
	while (x&bit_mask)
	{
		++one_cnt;
		bit_mask <<= 1;
	}
	x &= ~(bit_mask - 1);
	x |= bit_mask;
	while (--one_cnt > 0)
		x |= 1 << (one_cnt*2-1);
	return true;
}


int main()
{
	std::cout << "*** Test Catalan numbers generated by 2 template generators ***" << std::endl;
	std::cout << "Catalan(9) is : (" << static_cast<int64_t>(CATALAN<9, 9, false>::v::val) << ", " << static_cast<int64_t>(OTHER_CATALAN<9>::v::val) << ")" << std::endl;
	std::cout << "Catalan(15) is : (" << static_cast<int64_t>(CATALAN<15, 15, false>::v::val) << ", " << static_cast<int64_t>(OTHER_CATALAN<15>::v::val) << ")" << std::endl;
	std::cout << "Catalan(20) is : (" << static_cast<int64_t>(CATALAN<20, 20, false>::v::val) << ", " << static_cast<int64_t>(OTHER_CATALAN<20>::v::val) << ")" << std::endl;

	std::cout << std::endl;
	std::cout << "*** Test Catalan numbers generated by 3 different generators ***" << std::endl;
	for (int i = 0; i < 10; ++i)
		std::cout << "(" << catalan_number_dp(i) << ", " << catalan_number_recursive_1(i) << ", " << catalan_number_recursive_2(i, i) << ")" << std::endl;

	std::cout << std::endl;
	std::cout << "*** Test Catalan number sequences generated by 2 different DP generators ***" << std::endl;
	for (unsigned long x : catalan_seq_dp_1(10))
		std::cout << x << ", ";
	std::cout << std::endl;
	std::cout << "----------------------------------" << std::endl;
	for (unsigned long x : catalan_seq_dp_2(10))
		std::cout << x << ", ";
	std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "*** Test Catalan strings generated by 2 different generators ***" << std::endl;
	for (auto s : catalan_string_1(4))
		std::cout << "( " << s << ", " << is_catalan_seq(s.begin(), s.end()) << ")" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	std::vector<std::string> cat_strs;
	catalan_string_2(4, 4, "", cat_strs);
	for (auto s : cat_strs)
		std::cout << "( " << s << ", " << is_catalan_seq(s.begin(), s.end(), '(', ')') << ")" << std::endl;

	std::cout << std::endl;
	std::cout << "*** Test Catalan integers generated by 2 different generators ***" << std::endl;
	for (auto x : catalan_integer_1(4))
		std::cout << "(" << std::bitset<8>(x).to_string() << ", " << is_catalan_int(x, 4 * 2) << "), ";
	std::cout << std::endl;
	std::cout << "----------------------------------" << std::endl;
	std::vector<unsigned long> cat_longs;
	catalan_integer_2(4, 4, 0UL, cat_longs);
	for (auto x : cat_longs)
		std::cout << "(" << std::bitset<8>(x).to_string() << ", " << is_catalan_int(x, 4*2) << "), ";
	std::cout << std::endl;

	std::string s1 = "101100110010110010";
	std::string s2 = "101011000111001010";
	std::cout << is_catalan_seq(s1.begin(), s1.end()) << std::endl;
	std::cout << is_catalan_seq(s2.begin(), s2.end()) << std::endl;
	std::cout << is_catalan_int(0xff00, 16) << std::endl;	// 1111111100000000
	std::cout << is_catalan_int(0xf0f0, 16) << std::endl;	// 1111000011110000
	std::cout << is_catalan_int(0xcb8a, 16) << std::endl;	// 1100101110001010
	std::cout << is_catalan_int(0xcb86, 16) << std::endl;	// 1100101110000110

	unsigned int x = 0xf0; // 0x3e0; // 0x38;
	do {
		std::cout << std::bitset<8>(x) << std::endl;
	} while (next_catalan_int(x, 8));

	std::cout << std::endl;
	x = 0xaa; //0x2a;
	do {
		std::cout << std::bitset<8>(x) << std::endl;
	} while (prev_catalan_int(x, 8));

	return 0;
}

