#ifndef _COMBINATION_H_
#define _COMBINATION_H_

#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <cassert>

int lexi_sn_idx(int n, int k, int comb[]);
std::vector<int> nth_comb_idx(int lexi_sn);


//////////////////////////////////////////////////////////////////////
//
//    Calculate Combination Number
//
//////////////////////////////////////////////////////////////////////
template<int S, int M, int N> struct COMBO_SUM;
template<int M, int N> struct COMBINATION
{
	enum class v : int64_t { val = static_cast<int64_t>(COMBO_SUM<M - N + 1, M, N>::v::val) };
};
template<int M> struct COMBINATION<M, 1>
{
	enum class v : int64_t { val = M };
};
template<int S, int M, int N> struct COMBO_SUM
{
	enum class v : int64_t { val = static_cast<int64_t>(COMBINATION<M - S, N - 1>::v::val) + static_cast<int64_t>(COMBO_SUM<S - 1, M, N>::v::val) };
};
template<int M, int N> struct COMBO_SUM<0, M, N>
{
	enum class v : int64_t { val = 0 };
};


// Get the n-th order binomial coefficient using Dynamic Programming
// The n+1 numbers correspond to C(n, 0), C(n, 1), ......, C(n, n-1), C(n, n)
template<class RetIntType = unsigned int>
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


// calculate next combination
template<class BiIter>
bool next_combination(const BiIter seq_first, const BiIter seq_last, BiIter comb_first, BiIter comb_last)	// seq must be sorted
{
	int n = std::distance(seq_first, seq_last);
	int k = std::distance(comb_first, comb_last);
	assert(n >= k && k > 0);

	BiIter cur_pos = comb_last;
	--cur_pos;
	int chg_cnt = 0;
	while (true)
	{
		BiIter cur_val_itor = std::lower_bound(seq_first, seq_last, *cur_pos);
		++cur_val_itor;
		if (std::distance(cur_val_itor, seq_last) > chg_cnt)
		{
			while (cur_pos != comb_last)
				*cur_pos++ = *cur_val_itor++;
			return true;
		}
		else
		{
			++chg_cnt;
			if (chg_cnt < k)
				--cur_pos;
			else
			{
				for (int i = 0; i < k; ++i)
					*cur_pos++ = i;
				return false;
			}
		}
	}
}

template<class BiIter>
bool next_combination_idx(int n, BiIter comb_first, BiIter comb_last)
{
	int k = std::distance(comb_first, comb_last);
	assert(n >= k && k > 0);

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

/*
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
*/


////////////////////////////////////////////////////////////////////////////////
//
// The following 4 functions have been fully tested in both VS2013 and g++8.4,
// on signed and unsigned char, short, int, long long integer types, and works fine.
// This is a copy from the bit_operation projects, only because that these can
// be used to generate combinations, but the performance is not that fast,
// actually it is 1 times slower than the above next_combination_idx function.
//
////////////////////////////////////////////////////////////////////////////////
template<class IntType>
inline IntType first_n_1_int(int n)
{
	return static_cast<IntType>((IntType)(-1) >> (sizeof(IntType)*8-n));
}

template<class IntType>
inline IntType last_n_1_int(int n)
{
	return static_cast<IntType>((signed)((IntType)(-1)) & ~(((IntType)1<<(sizeof(IntType)*8-n))-1));
}

template<class IntType>
bool next_n_1_int(IntType& cur_x, int total_bits = sizeof(IntType)* 8)
{
	IntType bit_mask = 0x1;
	int one_cnt = 0;
	int cnt = 0;

	while (cnt++ < total_bits)
	{
		if (cur_x & bit_mask)
		{
			if (!(cur_x & (bit_mask << 1)))
				break;
			else
				++one_cnt;
		}
		bit_mask <<= 1;
	}
	bit_mask <<= 1;
	if (cnt >= total_bits)
		return false;
	cur_x |= bit_mask;
	cur_x &= ~(bit_mask - 1);
	cur_x |= ((1 << one_cnt) - 1);

	return true;
}

template<class IntType>
bool prev_n_1_int(IntType& cur_x, int total_bits = sizeof(IntType)* 8)
{
	IntType bit_mask = 0x1;
	int one_cnt = 0;
	int cnt = 0;
	while (cnt++ < total_bits)
	{
		if (cur_x & bit_mask)
			++one_cnt;
		else if(cur_x & (bit_mask << 1))
			break;
		bit_mask <<= 1;
	}
	if (cnt >= total_bits)
		return false;
	cur_x ^= (bit_mask << 1);
	cur_x |= bit_mask;
	cur_x &= ~(bit_mask - 1);
	cur_x |= (bit_mask - (bit_mask >> one_cnt));

	return true;
}

template<class BiIter>
bool next_comb_rotate(BiIter seq_begin, BiIter seq_end, int n)
{
	BiIter comb_last = seq_begin + n;
	BiIter remain;
	BiIter remain_end = seq_end;
	int mod_cnt = 0;
	while (true)
	{
		remain = comb_last;
		--comb_last;
		while (remain != remain_end)
		{
			if (*remain > *comb_last)
				break;
			else
				++remain;
		}
		if (remain == remain_end)
		{
			++mod_cnt;
			std::rotate(comb_last, comb_last + 1, remain_end);
			if (mod_cnt >= n)
				return false;
			//std::rotate(comb_last, comb_last + 1, seq_end);
			--remain_end;
		}
		else
			break;
	}
	if (mod_cnt == 0)
		std::iter_swap(comb_last, remain);
	else
	{
		if (*comb_last > *(comb_last + 1))
			std::rotate(comb_last, comb_last + 1, remain);
		std::rotate(comb_last, remain, remain + mod_cnt + 1);
	}
	return true;
}


class CombIdx
{
public:
	CombIdx() : n(0), k(0), first(nullptr), last(nullptr), cur_comb(nullptr) {};
	CombIdx(int _n, int _k) { setup(_n, _k); };
	template<class BiIter> CombIdx(int _n, int _k, BiIter init_comb_begin) { setup(_n, _k, init_comb_begin); };
	template<class BiIter> CombIdx(int _n, BiIter init_comb_begin, BiIter init_comb_end) { setup(_n, init_comb_begin, init_comb_end); };
	~CombIdx() { delete[] first; delete[] last; delete[] cur_comb; };

	void reset(void);
	void setup(int _n, int _k);
	template<class BiIter> void setup(int _n, int _k, BiIter init_comb_begin);
	template<class BiIter> void setup(int _n, BiIter init_comb_begin, BiIter init_comb_end);
	bool next_comb_idx(void);
	bool prev_comb_idx(void);
	const std::vector<int> get_first(void) const { return std::vector<int> (first, first+k); };
	const std::vector<int> gett_last(void) const { return std::vector<int>(last, last + k); };
	const std::vector<int> get_cur_comb(void) const { return std::vector<int>(cur_comb, cur_comb + k); };
	void set_cur_comb(int comb[]);
	
private:
	int n;
	int k;
	int *first;
	int *last;
	int *cur_comb;
};

void CombIdx::reset(void)
{
	if (n != 0)
	{
		n = 0;
		k = 0;
		delete[] first;
		first = nullptr;
		delete[] last;
		last = nullptr;
		delete[] cur_comb;
		cur_comb = nullptr;
	}
}

void CombIdx::setup(int _n, int _k)
{
	n = _n;
	k = _k;
	first = new int[_k];
	last = new int[_k];
	cur_comb = new int[_k];
/*	
	for (int i = 0; i<_k; ++i)
	{
		first[i] =i;
		last[i] = _n - _k + i;
		cur_comb[i] = i;
	}
*/
	std::iota(first, first + k, 0);
	std::iota(last, last + k, n - k);
	std::iota(cur_comb, cur_comb + k, 0);
}

template<class BiIter> void CombIdx::setup(int _n, int _k, BiIter init_comb_begin)
{
	setup(_n, _k);
	BiIter init_iter = init_comb_begin;
	for (int i = 0; i < k; ++i)
		cur_comb[i] = *init_iter++;
}

template<class BiIter> void CombIdx::setup(int _n, BiIter init_comb_begin, BiIter init_comb_end)
{
	setup(_n, std::distance(init_comb_begin, init_comb_end), init_comb_begin);
}

/*
bool CombIdx::next_comb_idx(void)
{
	if (cur_comb[0] == n - k)
		return false;
	int cur_pos = k - 1;
	int cur_max = n - 1;
	while (true)
	{
		int cur_idx = cur_comb[cur_pos] + 1;
		if (cur_idx <= cur_max)
		{
			while (cur_pos < k)
				cur_comb[cur_pos++] = cur_idx++;
			return true;
		}
		else
		{
			--cur_pos;
			--cur_max;
		}
	}
}
*/

bool CombIdx::next_comb_idx(void)
{
	if (cur_comb[0] == n - k)
		return false;
	int *comb_end = cur_comb + k;
	int *cur_pos = comb_end - 1;
	int cur_max = n - 1;
	while (true)
	{
		int cur_idx = *cur_pos + 1;
		if (cur_idx <= cur_max)
		{
			// Using STL algorithm is not always good for performance, especially when you have to pay a big price of calling a function
			// with 3 params than normal pointer operation. I just use it here to remember this function and for comparison.
			std::iota(cur_pos, comb_end, cur_idx);
			return true;
		}
		else
		{
			--cur_pos;
			--cur_max;
		}
	}
}

/*
bool CombIdx::prev_comb_idx(void)
{
	if (cur_comb[k - 1] == k - 1)
		return false;
	int cur_pos = k - 1;
	int cur_max = n;
	while (true)
	{
		int cur_idx = cur_comb[cur_pos] - 1;
		if (cur_pos==0 || cur_idx > cur_comb[cur_pos - 1])
		{
			cur_comb[cur_pos++] = cur_idx;
			while (cur_pos < k)
				cur_comb[cur_pos++] = cur_max++;
			return true;
		}
		else
		{
			--cur_pos;
			--cur_max;
		}
	}
}
*/

bool CombIdx::prev_comb_idx(void)
{
	if (cur_comb[k - 1] == k - 1)
		return false;
	int *comb_end = cur_comb + k;
	int *cur_pos = comb_end - 1;
	int cur_max = n;
	while (true)
	{
		int cur_idx = *cur_pos - 1;
		if (cur_pos==cur_comb || cur_idx > *(cur_pos - 1))
		{
			*cur_pos++ = cur_idx;
			std::iota(cur_pos, comb_end, cur_max);
			return true;
		}
		else
		{
			--cur_pos;
			--cur_max;
		}
	}
}

void CombIdx::set_cur_comb(int comb[])
{
	for (int i = 0; i < k; ++i)
		cur_comb[i] = comb[i];
}



template<class T> 
class CombSeq
{
public:
	CombSeq() : n(0), k(0), first(nullptr), last(nullptr), cur_comb(nullptr), full_seq(nullptr) {};
	template<class BiIter> CombSeq(int _n, int _k, BiIter full_seq_sorted_begin) { setup(_n, _k, full_seq_sorted_begin); };
	template<class BiIter1, class BiIter2> CombSeq(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_comb_begin) { setup(_n, _k, full_seq_sorted_begin, init_comb_begin); };
	template<class BiIter> CombSeq(BiIter full_seq_begin, BiIter full_seq_end, int _k) { setup(full_seq_begin, full_seq_end, _k); };
	template<class BiIter1, class BiIter2> CombSeq(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_comb_begin, BiIter2 init_comb_end) { setup(full_seq_begin, full_seq_end, init_comb_begin, init_comb_end); };
	~CombSeq() { delete[] first; delete[] last; delete[] cur_comb; delete[] full_seq; }
	void reset(void);
	template<class BiIter> void setup(int _n, int _k, BiIter full_seq_sorted_begin);
	template<class BiIter1, class BiIter2> void setup(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_comb_begin);
	template<class BiIter> void setup(BiIter full_seq_begin, BiIter full_seq_end, int _k);
	template<class BiIter1, class BiIter2> void setup(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_comb_begin, BiIter2 init_comb_end);
	bool next_comb_seq(void);
	bool prev_comb_seq(void);
	const std::vector<T> get_first(void) const { return std::vector<T>(first, first+k); };
	const std::vector<T> get_last(void) const { return std::vector<T>(last, last + k); };
	const std::vector<T> get_cur_comb(void) const { return std::vector<T>(cur_comb, cur_comb + k); };
	void set_cur_comb(T comb[]);
	
private:
	int n;
	int k;
	T *first;
	T *last;
	T *cur_comb;
	T * full_seq;
};

template<class T>
void CombSeq<T>::reset(void)
{
	if (n != 0)
	{
		n = 0;
		k = 0;
		delete[] first;
		first = nullptr;
		delete[] last;
		last = nullptr;
		delete[] cur_comb;
		cur_comb = nullptr;
		delete[] full_seq;
		full_seq = nullptr;
	}
}

template<class T>
template<class BiIter>
void CombSeq<T>::setup(int _n, int _k, BiIter full_seq_sorted_begin)
{
	n = _n;
	k = _k;
	first = new T[_k];
	last = new T[_k];
	cur_comb = new T[_k];
	full_seq = new T[_n];
	
	BiIter fss_iter = full_seq_sorted_begin;
	for (int i = 0; i<_n; ++i)
		full_seq[i] = *fss_iter++;
		
	T *forward_ptr = full_seq;
	T *backward_ptr = full_seq + n - k;

	for (int i = 0; i<_k; ++i)
	{
		first[i] = *forward_ptr;
		cur_comb[i] = *forward_ptr++;
		last[i] = *backward_ptr++;
	}
}

template<class T>
template<class BiIter1, class BiIter2>
void CombSeq<T>::setup(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_comb_begin)
{
	setup(_n, _k, full_seq_sorted_begin);
	BiIter2 init_iter = init_comb_begin;
	for (int i = 0; i < k; ++i)
		cur_comb[i] = *init_iter++;
};

template<class T>
template<class BiIter>
void CombSeq<T>::setup(BiIter full_seq_begin, BiIter full_seq_end, int _k)
{
	setup(std::distance(full_seq_begin, full_seq_end), _k, full_seq_begin);

}

template<class T>
template<class BiIter1, class BiIter2>
void CombSeq<T>::setup(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_comb_begin, BiIter2 init_comb_end)
{
	setup(std::distance(full_seq_begin, full_seq_end), std::distance(init_comb_begin, init_comb_end), full_seq_begin, init_comb_begin);
}

template<class T>
bool CombSeq<T>::next_comb_seq(void)
{
	if (cur_comb[0] == full_seq[n - k])
		return false;
	int cur_pos = k - 1;
	int chg_cnt = 0;
	while (true)
	{
		auto itr = std::upper_bound(full_seq, full_seq+n, cur_comb[cur_pos]);
		if (std::distance(itr, full_seq+n) > chg_cnt)
		{
			while (cur_pos < k)
				cur_comb[cur_pos++] = *itr++;
			return true;
		}
		else
		{
			--cur_pos;
			++chg_cnt;
		}
	}
}

template<class T>
bool CombSeq<T>::prev_comb_seq(void)
{
	if (cur_comb[k - 1] == full_seq[k - 1])
		return false;
	int cur_pos = k - 1;
	int fill_idx = n;
	while (true)
	{
		auto itr = std::lower_bound(full_seq, full_seq+n, cur_comb[cur_pos]);
		if (cur_pos == 0 || *(itr - 1) > cur_comb[cur_pos - 1])
		{
			cur_comb[cur_pos++] = *(itr - 1);
			itr = full_seq + fill_idx;
			while (cur_pos < k)
				cur_comb[cur_pos++] = *itr++;
			return true;
		}
		else
		{
			--cur_pos;
			--fill_idx;
		}
	}
}

template<class T>
void CombSeq<T>::set_cur_comb(T comb[])
{
	for (int i = 0; i < k; ++i)
		cur_comb[i] = comb[i];
}

//template CombSeq<char>::CombSeq(int, int, char *);

#endif
