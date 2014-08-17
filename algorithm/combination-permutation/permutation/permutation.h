#ifndef _PERMUTATION_H_
#define _PERMUTATION_H_

#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>


//
// Calculate Permutation number
//
template <int M, int N> struct PERMUTATION
{
	enum class v : int64_t { val = M * static_cast<int64_t>(PERMUTATION<M-1, N-1>::v::val) };
};
template<int M> struct PERMUTATION<M, 0>
{
	enum class v : int64_t { val = 1 };
};

// The maximum length of the sequence to be permuted, used in the idx_seq_to_lexi_order and lexi_order_to_idx_seq functions
#define MAX_PERMUTATION_SEQ_LEN	12
// factorial table from 0 to 12
static const unsigned long fac_table[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600 };

// Calculate the lexicographic permutation index from permutation's index sequence
unsigned long idx_seq_to_lexi_order(int idx_seq[], int N)
{
	if (N <= 1)
		return 0UL;
	int coeffs[MAX_PERMUTATION_SEQ_LEN];
	int *seq_ptr = idx_seq;
	int *coeff_ptr = coeffs + N - 2;

	while(seq_ptr != idx_seq + N - 1)
	{
		int order = *seq_ptr;
		order -= std::count_if(idx_seq, seq_ptr, [order](int x) { return x < order; });
		*coeff_ptr-- = order;
		++seq_ptr;
	}
	unsigned long sn = coeffs[0];
	coeff_ptr = coeffs;
	++coeff_ptr;
	for (int i = 2; i <= N - 1; ++i)
	{
		if (*coeff_ptr)
			sn += *coeff_ptr * fac_table[i];
		++coeff_ptr;
	}
	return sn;
}

// Calculate the lexicographic permutation index from permutation's index sequence
std::vector<int> lexi_order_to_idx_seq(unsigned long lexi_order, int N)
{
	std::vector<int> idx_seq;
	auto fac_iter = std::upper_bound(fac_table, fac_table + N + 1, lexi_order);
	int n = std::distance(fac_table, fac_iter);
	for (int i = 0; i < N - n; ++i)
		idx_seq.push_back(i);
	--fac_iter;
	for (int i = 0; i < n; ++i)
	{
		int coeff = lexi_order / *fac_iter;
		lexi_order -= coeff * *fac_iter--;
		int idx = 0;
		while(idx < N)
		{
			if (std::find(idx_seq.begin(), idx_seq.end(), idx) == idx_seq.end())
				--coeff;
			if (coeff < 0)
				break;
			++idx;
		}
		idx_seq.push_back(idx);
	}
	return idx_seq;
}

//
// This code used ancient Hindu mathematician Narayana Pandit's permutation algorithm,
// and its performance is comparable with std::next_permutation, only that it can only
// P(n, n), cannot do P(n, k)
//
// The explain of the code can be found here:
// http://stackoverflow.com/questions/11483060/stdnext-permutation-implementation-explanation
//
template<class BiIter>
bool next_perm(BiIter first, BiIter last)
{
	if (first == last) return false;
	BiIter i = last;
	if (first == --i) return false;

	while (1)
	{
		BiIter i1, i2;

		i1 = i;
		if (*--i < *i1)	// Find the position of the first neighboring position [...i, i1...] is in ascending order
		{
			i2 = last;
			while (!(*i < *--i2))	// Finding the first number greater than *i,(and is the greatest)in the right to be swapped with *i
				;
			std::iter_swap(i, i2);	// Swap *i and *i2
			std::reverse(i1, last);	// reverse the position after i till end
			return true;
		}
		if (i == first)
		{
			std::reverse(first, last);	// I reverse the last permutation to the first permutation only to conform with std::next_permutation, would rather not to reverse it. 
			return false;
		}
	}
}


class PermIdx
{
public:
	PermIdx() : n(0), k(0), first(nullptr), last(nullptr), cur_perm(nullptr), lookup_table(nullptr) {};
	PermIdx(int _n, int _k) { setup(_n, _k); };
	template<class BiIter> PermIdx(int _n, int _k, BiIter init_perm_begin) { setup(_n, _k, init_perm_begin); };
	template<class BiIter> PermIdx(int _n, BiIter init_perm_begin, BiIter init_perm_end) { setup(_n, init_perm_begin, init_perm_end); };
	~PermIdx() { reset(); };
	
	void reset(void);
	void setup(int _n, int _k);
	template<class BiIter> void setup(int _n, int _k, BiIter init_perm_begin);
	template<class BiIter> void setup(int _n, BiIter init_perm_begin, BiIter init_perm_end);
	bool next_perm_idx(void);
	bool prev_perm_idx(void);
	const std::vector<int> get_first(void) const { return std::vector<int>(first, first + k); };
	const std::vector<int> get_last(void) const { return std::vector<int>(last, last + k); };
	const std::vector<int> get_cur_perm(void) const { return std::vector<int>(cur_perm, cur_perm + k); };
	void set_cur_perm(int perm[]);

private:
	int n;
	int k;
	int *first;
	int *last;
	int *cur_perm;
	int *lookup_table;
};

void PermIdx::reset(void)
{
	if (n != 0)
	{
		n = 0;
		k = 0;
		delete[] first;
		first = nullptr;
		delete[] last;
		last = nullptr;
		delete[] cur_perm;
		cur_perm = nullptr;
		delete[] lookup_table;
		lookup_table = nullptr;
	}
}

void PermIdx::setup(int _n, int _k)
{
	n = _n;
	k = _k;
	first = new int[_k];
	last = new int[_k];
	cur_perm = new int[_k];
	lookup_table = new int[_n];
	//for (int i = 0; i < _n; ++i)
	//	lookup_table[i] = 0;
	std::fill(lookup_table, lookup_table + n, 0);
/*
	for (int i = 0; i<_k; ++i)
	{
		first[i] = i;
		last[i] = _n - 1 - i;
		cur_perm[i] = i;
		lookup_table[i]++;
	}
*/
	std::iota(first, first + k, 0);
	std::iota(cur_perm, cur_perm + k, 0);
	for (int i = 0; i<_k; ++i)
	{
		last[i] = _n - 1 - i;
		lookup_table[i]++;
	}
}

template<class BiIter> void PermIdx::setup(int _n, int _k, BiIter init_perm_begin)
{
	setup(_n, _k);
	BiIter forward_iter = init_perm_begin;
	for (int i = 0; i < k; ++i)
	{
		cur_perm[i] = *forward_iter++;
		++lookup_table[cur_perm[i]];
	}
}

template<class BiIter> void PermIdx::setup(int _n, BiIter init_perm_begin, BiIter init_perm_end)
{
	setup(_n, std::distance(init_perm_begin, init_perm_end), init_perm_begin);
}


bool PermIdx::next_perm_idx(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int cur_idx = cur_perm[cur_pos];
		--lookup_table[cur_idx++];
		while (cur_idx < n && lookup_table[cur_idx])
			++cur_idx;
		if (cur_idx < n)
		{
			cur_perm[cur_pos++] = cur_idx;
			++lookup_table[cur_idx];
			int lookup_table_idx = 0;
			while (cur_pos < k)
			{
				while (lookup_table[lookup_table_idx])
					++lookup_table_idx;
				cur_perm[cur_pos++] = lookup_table_idx;
				++lookup_table[lookup_table_idx++];
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 0; i < k; ++i)
					++lookup_table[n-1-i];
				return false;
			}
		}
	}
}

bool PermIdx::prev_perm_idx(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int cur_idx = cur_perm[cur_pos];
		--lookup_table[cur_idx--];
		while (cur_idx >= 0 && lookup_table[cur_idx])
			--cur_idx;
		if (cur_idx >= 0)
		{
			cur_perm[cur_pos++] = cur_idx;
			++lookup_table[cur_idx];
			int lookup_table_idx = n - 1;
			while (cur_pos < k)
			{
				while (lookup_table[lookup_table_idx])
					--lookup_table_idx;
				cur_perm[cur_pos++] = lookup_table_idx;
				++lookup_table[lookup_table_idx--];
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 0; i < k; ++i)
					++lookup_table[i];
				return false;
			}
		}
	}
}

// Almost the same as PermIdx, only that it uses bit operation for table lookup.
// It uses an unsigned integer(32 bit) as lookup table, so, it can permuted
// from at most 32 indexes. The advantage is it only uses an integer instead of
// an integer array. The performance is also almost the same with PermIdx 
class PermIdxBit
{
public:
	PermIdxBit() : n(0), k(0), lookup_table(0), first(nullptr), last(nullptr), cur_perm(nullptr) {};
	PermIdxBit(int _n, int _k) : lookup_table(0) { setup(_n, _k); };
	template<class BiIter> PermIdxBit(int _n, int _k, BiIter init_perm_begin) : lookup_table(0) { setup(_n, _k, init_perm_begin); };
	template<class BiIter> PermIdxBit(int _n, BiIter init_perm_begin, BiIter init_perm_end) : lookup_table(0) { setup(_n, init_perm_begin, init_perm_end); };
	~PermIdxBit() { reset(); };

	void reset(void);
	void setup(int _n, int _k);
	template<class BiIter> void setup(int _n, int _k, BiIter init_perm_begin);
	template<class BiIter> void setup(int _n, BiIter init_perm_begin, BiIter init_perm_end);
	bool next_perm_idx(void);
	bool prev_perm_idx(void);
	const std::vector<int> get_first(void) const { return std::vector<int>(first, first + k); };
	const std::vector<int> get_last(void) const { return std::vector<int>(last, last + k); };
	const std::vector<int> get_cur_perm(void) const { return std::vector<int>(cur_perm, cur_perm + k); };
	void set_cur_perm(int perm[]);

private:
	int n;
	int k;
	unsigned int lookup_table;
	int *first;
	int *last;
	int *cur_perm;
};

void PermIdxBit::reset(void)
{
	if (n != 0)
	{
		n = 0;
		k = 0;
		delete[] first;
		first = nullptr;
		delete[] last;
		last = nullptr;
		delete[] cur_perm;
		cur_perm = nullptr;
		lookup_table = 0;
	}
}

void PermIdxBit::setup(int _n, int _k)
{
	n = _n;
	k = _k;
	first = new int[_k];
	last = new int[_k];
	cur_perm = new int[_k];

	std::iota(first, first + k, 0);
	std::iota(cur_perm, cur_perm + k, 0);
	lookup_table |= (1 << k) - 1;
	for (int i = 0; i<_k; ++i)
		last[i] = _n - 1 - i;
}

template<class BiIter> void PermIdxBit::setup(int _n, int _k, BiIter init_perm_begin)
{
	setup(_n, _k);
	BiIter forward_iter = init_perm_begin;
	for (int i = 0; i < k; ++i)
	{
		cur_perm[i] = *forward_iter++;
		lookup_table |= 1<<cur_perm[i];
	}
}

template<class BiIter> void PermIdxBit::setup(int _n, BiIter init_perm_begin, BiIter init_perm_end)
{
	setup(_n, std::distance(init_perm_begin, init_perm_end), init_perm_begin);
}

bool PermIdxBit::next_perm_idx(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int cur_idx = cur_perm[cur_pos];
		unsigned int cur_bit_mask = 1 << cur_idx;
		lookup_table ^= cur_bit_mask;
		++cur_idx;
		cur_bit_mask <<= 1;

		while (cur_idx < n && (lookup_table & cur_bit_mask))
		{
			++cur_idx;
			cur_bit_mask <<= 1;
		}
		if (cur_idx < n)
		{
			cur_perm[cur_pos++] = cur_idx;
			lookup_table |= cur_bit_mask;
			int lookup_table_idx = 0;
			cur_bit_mask = 1;
			while (cur_pos < k)
			{
				while (lookup_table & cur_bit_mask)
				{
					++lookup_table_idx;
					cur_bit_mask <<= 1;
				}
				cur_perm[cur_pos++] = lookup_table_idx;
				lookup_table |= cur_bit_mask;
				++lookup_table_idx;
				cur_bit_mask <<= 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				lookup_table = ((1<<k)-1) << (n - k);
				return false;
			}
		}
	}
}

bool PermIdxBit::prev_perm_idx(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int cur_idx = cur_perm[cur_pos];
		unsigned int cur_bit_mask = 1 << cur_idx;
		lookup_table ^= cur_bit_mask;
		--cur_idx;
		cur_bit_mask >>= 1;

		while (cur_idx >= 0 && (lookup_table & cur_bit_mask))
		{
			--cur_idx;
			cur_bit_mask >>= 1;
		}
		if (cur_idx >= 0)
		{
			cur_perm[cur_pos++] = cur_idx;
			lookup_table |= cur_bit_mask;
			int lookup_table_idx = n - 1;
			cur_bit_mask = 1 << lookup_table_idx;
			while (cur_pos < k)
			{
				while (lookup_table & cur_bit_mask)
				{
					--lookup_table_idx;
					cur_bit_mask >>= 1;
				}
				cur_perm[cur_pos++] = lookup_table_idx;
				lookup_table |= cur_bit_mask;
				--lookup_table_idx;
				cur_bit_mask >>= 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				lookup_table = (1 << k) - 1;
				return false;
			}
		}
	}
}


template<class T>
class PermSeq
{
public:
	PermSeq() : n(0), k(0), first(nullptr), last(nullptr), cur_perm(nullptr), lookup_table(nullptr), full_seq(nullptr) {};
	template<class BiIter> PermSeq(int _n, int _k, BiIter full_seq_sorted_begin) { setup(_n, _k, full_seq_sorted_begin); };
	template<class BiIter1, class BiIter2> PermSeq(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_perm_begin) { setup(_n, _k, full_seq_sorted_begin, init_perm_begin); };
	template<class BiIter> PermSeq(BiIter full_seq_begin, BiIter full_seq_end, int _k) { setup(full_seq_begin, full_seq_end, _k); };
	template<class BiIter1, class BiIter2> PermSeq(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_perm_begin, BiIter2 init_perm_end) { setup(full_seq_begin, full_seq_end, init_perm_begin, init_perm_end); };
	~PermSeq() { reset(); };

	void reset(void);
	template<class BiIter> void setup(int _n, int _k, BiIter full_seq_sorted);
	template<class BiIter1, class BiIter2> void setup(int _n, int _k, BiIter1 full_seq_sorted, BiIter2 init_perm);
	template<class BiIter> void setup(BiIter full_seq_begin, BiIter full_seq_end, int _k);
	template<class BiIter1, class BiIter2> void setup(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_perm_begin, BiIter2 init_perm_end);
	bool next_perm_seq(void);
	bool prev_perm_seq(void);
	const std::vector<T> get_first(void) const { return std::vector<T>(first, first+k); };
	const std::vector<T> get_last(void) const { return std::vector<T>(last, last + k); };
	const std::vector<T> get_cur_perm(void) const { return std::vector<T>(cur_perm, cur_perm + k); };
	void set_cur_perm(T perm[]);

private:
	int n;
	int k;
	T *first;
	T *last;
	T *cur_perm;
	T *full_seq;
	int *lookup_table;
};

template<class T>
void PermSeq<T>::reset(void)
{
	if (n != 0)
	{
		n = 0;
		k = 0;
		delete[] first;
		first = nullptr;
		delete[] last;
		last = nullptr;
		delete[] cur_perm;
		cur_perm = nullptr;
		delete[] lookup_table;
		lookup_table = nullptr;
		delete[] full_seq;
		full_seq = nullptr;
	}
}

template<class T>
template<class BiIter>
void PermSeq<T>::setup(int _n, int _k, BiIter full_seq_sorted_begin)
{
	n = _n;
	k = _k;
	first = new T[_k];
	last = new T[_k];
	cur_perm = new T[_k];
	lookup_table = new int[_n];
	std::fill_n(lookup_table, n, 0);
	full_seq = new T[_n];
	BiIter forward_iter = full_seq_sorted_begin;
	for (int i = 0; i < _n; ++i)
		full_seq[i] = *forward_iter++;
	T *forward_ptr = full_seq;
	T *backward_ptr = full_seq + n -1;
	for (int i = 0; i<_k; ++i)
	{
		first[i] = *forward_ptr;
		cur_perm[i] = *forward_ptr++;
		last[i] = *backward_ptr--;
		lookup_table[i]++;
	}
}

template<class T>
template<class BiIter1, class BiIter2>
void PermSeq<T>::setup(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_perm_begin)
{
	setup(_n, _k, full_seq_sorted_begin);
	BiIter2 init_iter = init_perm_begin;
	for (int i = 0; i < k; ++i)
	{
		cur_perm[i] = *init_iter++;
		auto itr = std::lower_bound(full_seq, full_seq + _n, cur_perm[i]);
		++lookup_table[std::distance(full_seq, itr)];
	}
}

template<class T>
template<class BiIter>
void PermSeq<T>::setup(BiIter full_seq_begin, BiIter full_seq_end, int _k)
{
	setup(std::distance(full_seq_begin, full_seq_end), _k, full_seq_begin);
}

template<class T>
template<class BiIter1, class BiIter2>
void PermSeq<T>::setup(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_perm_begin, BiIter2 init_perm_end)
{
	setup(std::distance(full_seq_begin, full_seq_end), std::distance(init_perm_begin, init_perm_end), full_seq_begin, init_perm_begin);
}

template<class T>
bool PermSeq<T>::next_perm_seq(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int table_idx = std::distance(full_seq, std::upper_bound(full_seq, full_seq+n, cur_perm[cur_pos]));
		--lookup_table[table_idx - 1];
		while (table_idx < n && lookup_table[table_idx])
			++table_idx;
		if (table_idx < n)
		{
			cur_perm[cur_pos++] = full_seq[table_idx];
			++lookup_table[table_idx];
			table_idx = 0;
			while (cur_pos < k)
			{
				while (lookup_table[table_idx])
					++table_idx;
				cur_perm[cur_pos++] = full_seq[table_idx];
				++lookup_table[table_idx++];
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 0; i < k; ++i)
					++lookup_table[n-1-i];
				return false;
			}
		}
	}
}

template<class T>
bool PermSeq<T>::prev_perm_seq(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int table_idx = std::distance(full_seq, std::lower_bound(full_seq, full_seq+n, cur_perm[cur_pos]));
		--lookup_table[table_idx--];
		while (table_idx >= 0 && lookup_table[table_idx])
			--table_idx;
		if (table_idx >= 0)
		{
			cur_perm[cur_pos++] = full_seq[table_idx];
			++lookup_table[table_idx];
			table_idx = n - 1;
			while (cur_pos < k)
			{
				while (lookup_table[table_idx])
					--table_idx;
				cur_perm[cur_pos++] = full_seq[table_idx];
				++lookup_table[table_idx--];
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 0; i < k; ++i)
					++lookup_table[i];
				return false;
			}
		}
	}
}


// Almost the same as PermIdx, only that it uses bit operation for table lookup.
// It uses an unsigned integer(32 bit) as lookup table, so, it can permuted
// from at most 32 indexes. The advantage is it only uses an integer instead of
// an integer array. The performance is also almost the same with PermIdx 
template<class T> class PermSeqBit
{
public:
	PermSeqBit() : n(0), k(0), lookup_table(0), first(nullptr), last(nullptr), cur_perm(nullptr), full_seq(nullptr) {};
	template<class BiIter> PermSeqBit(int _n, int _k, BiIter full_seq_sorted_begin) : lookup_table(0) { setup(_n, _k, full_seq_sorted_begin); };
	template<class BiIter1, class BiIter2> PermSeqBit(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_perm_begin) : lookup_table(0) { setup(_n, _k, full_seq_sorted_begin, init_perm_begin); };
	template<class BiIter> PermSeqBit(BiIter full_seq_begin, BiIter full_seq_end, int _k) : lookup_table(0) { setup(full_seq_begin, full_seq_end, _k); };
	template<class BiIter1, class BiIter2> PermSeqBit(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_perm_begin, BiIter2 init_perm_end) : lookup_table(0) { setup(full_seq_begin, full_seq_end, init_perm_begin, init_perm_end); };
	~PermSeqBit() { reset(); };

	void reset(void);
	template<class BiIter> void setup(int _n, int _k, BiIter full_seq_sorted);
	template<class BiIter1, class BiIter2> void setup(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_perm_begin);
	template<class BiIter> void setup(BiIter full_seq_begin, BiIter full_seq_end, int _k);
	template<class BiIter1, class BiIter2> void setup(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_perm_begin, BiIter2 init_perm_end);
	bool next_perm_seq(void);
	bool prev_perm_seq(void);
	const std::vector<T> get_first(void) const { return std::vector<T>(first, first + k); };
	const std::vector<T> get_last(void) const { return std::vector<T>(last, last + k); };
	const std::vector<T> get_cur_perm(void) const { return std::vector<T>(cur_perm, cur_perm + k); };
	void set_cur_perm(T perm[]);

private:
	int n;
	int k;
	unsigned int lookup_table;
	T *first;
	T *last;
	T *cur_perm;
	T *full_seq;
};

template<class T>
void PermSeqBit<T>::reset(void)
{
	if (n != 0)
	{
		n = 0;
		k = 0;
		delete[] first;
		first = nullptr;
		delete[] last;
		last = nullptr;
		delete[] cur_perm;
		cur_perm = nullptr;
		delete[] full_seq;
		full_seq = nullptr;
		lookup_table = 0;
	}
}

template<class T>
template<class BiIter>
void PermSeqBit<T>::setup(int _n, int _k, BiIter full_seq_sorted_begin)
{
	n = _n;
	k = _k;
	first = new T[_k];
	last = new T[_k];
	cur_perm = new T[_k];

	full_seq = new T[_n];
	BiIter forward_iter = full_seq_sorted_begin;
	for (int i = 0; i < _n; ++i)
		full_seq[i] = *forward_iter++;

	T *forward_ptr = full_seq;
	T *backward_ptr = full_seq + _n - 1;
	for (int i = 0; i<_k; ++i)
	{
		first[i] = *forward_ptr;
		cur_perm[i] = *forward_ptr++;
		last[i] = *backward_ptr--;
	}
	lookup_table |= (1 << k) - 1;
}

template<class T>
template<class BiIter1, class BiIter2>
void PermSeqBit<T>::setup(int _n, int _k, BiIter1 full_seq_sorted_begin, BiIter2 init_perm_begin)
{
	setup(_n, _k, full_seq_sorted_begin);
	BiIter2 init_iter = init_perm_begin;
	for (int i = 0; i < k; ++i)
	{
		cur_perm[i] = *init_iter++;
		auto itr = std::lower_bound(full_seq, full_seq + _n, cur_perm[i]);
		lookup_table |= 1 << std::distance(full_seq, itr);

	}
}

template<class T>
template<class BiIter>
void PermSeqBit<T>::setup(BiIter full_seq_begin, BiIter full_seq_end, int _k)
{
	setup(std::distance(full_seq_begin, full_seq_end), _k, full_seq_begin);
}

template<class T>
template<class BiIter1, class BiIter2>
void PermSeqBit<T>::setup(BiIter1 full_seq_begin, BiIter1 full_seq_end, BiIter2 init_perm_begin, BiIter2 init_perm_end)
{
	setup(std::distance(full_seq_begin, full_seq_end), std::distance(init_perm_begin, init_perm_end), full_seq_begin, init_perm_begin);
}

template<class T>
bool PermSeqBit<T>::next_perm_seq(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int table_idx = std::distance(full_seq, std::upper_bound(full_seq, full_seq + n, cur_perm[cur_pos]));
		unsigned int cur_bit_mask = 1 << table_idx;
		lookup_table ^= (cur_bit_mask >> 1);
		while (table_idx < n && (lookup_table & cur_bit_mask))
		{
			++table_idx;
			cur_bit_mask <<= 1;
		}
		if (table_idx < n)
		{
			cur_perm[cur_pos++] = full_seq[table_idx];
			lookup_table |= cur_bit_mask;
			table_idx = 0;
			cur_bit_mask = 1;
			while (cur_pos < k)
			{
				while (lookup_table & cur_bit_mask)
				{
					++table_idx;
					cur_bit_mask <<= 1;
				}
				cur_perm[cur_pos++] = full_seq[table_idx];
				lookup_table |= cur_bit_mask;
				++table_idx;
				cur_bit_mask <<= 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				lookup_table = ((1 << k) - 1) << (n - k);
				return false;
			}
		}
	}
}

template<class T>
bool PermSeqBit<T>::prev_perm_seq(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int table_idx = std::distance(full_seq, std::lower_bound(full_seq, full_seq + n, cur_perm[cur_pos]));
		unsigned int cur_bit_mask = 1 << table_idx;
		lookup_table ^= cur_bit_mask;
		--table_idx;
		cur_bit_mask >>= 1;
		while (table_idx >= 0 && (lookup_table & cur_bit_mask))
		{
			--table_idx;
			cur_bit_mask >>= 1;
		}
		if (table_idx >= 0)
		{
			cur_perm[cur_pos++] = full_seq[table_idx];
			lookup_table |= cur_bit_mask;
			table_idx = n - 1;
			cur_bit_mask = 1 << table_idx;
			while (cur_pos < k)
			{
				while (lookup_table & cur_bit_mask)
				{
					--table_idx;
					cur_bit_mask >>= 1;
				}
				cur_perm[cur_pos++] = full_seq[table_idx];
				lookup_table |= cur_bit_mask;
				--table_idx;
				cur_bit_mask >>= 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				lookup_table = (1 << k) - 1;
				return false;
			}
		}
	}
}

#endif
