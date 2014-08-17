#ifndef _PERMUTATION_H_
#define _PERMUTATION_H_

#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

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
template<class BidirIt>
bool next_perm(BidirIt first, BidirIt last)
{
	if (first == last) return false;
	BidirIt i = last;
	if (first == --i) return false;

	while (1)
	{
		BidirIt i1, i2;

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
	PermIdx(int _n, int _k, int *init_perm) { setup(_n, _k, init_perm); };
	~PermIdx() { reset(); };
	
	void reset(void);
	void setup(int _n, int _k);
	void setup(int _n, int _k, int *init_perm);
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

void PermIdx::setup(int _n, int _k, int *init_perm)
{
	setup(_n, _k);
	for (int i = 0; i < k; ++i)
	{
		cur_perm[i] = init_perm[i];
		lookup_table[init_perm[i]]++;
	}
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

template<class T>
class PermSeq
{
public:
	PermSeq() : n(0), k(0), first(nullptr), last(nullptr), cur_perm(nullptr), lookup_table(nullptr), full_seq(nullptr) {};
	PermSeq(int _n, int _k, T *full_seq_sorted) { setup(_n, _k, full_seq_sorted); };
	PermSeq(int _n, int _k, T *full_seq_sorted, T *init_perm) { setup(_n, _k, full_seq_sorted, init_perm); };
	PermSeq(T *full_seq_begin, T *full_seq_end, int _k) { setup(full_seq_begin, full_seq_end, _k); };
	PermSeq(T *full_seq_begin, T *full_seq_end, T *init_perm_begin, T *init_perm_end) { setup(full_seq_begin, full_seq_end, init_perm_begin, init_perm_end); };
	~PermSeq() { reset(); };

	void reset(void);
	void setup(int _n, int _k, T *full_seq_sorted);
	void setup(int _n, int _k, T *full_seq_sorted, T *init_perm);
	void setup(T *full_seq_begin, T *full_seq_end, int _k);
	void setup(T *full_seq_begin, T *full_seq_end, T *init_perm_begin, T *init_perm_end);
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
void PermSeq<T>::setup(int _n, int _k, T *full_seq_sorted)
{
	n = _n;
	k = _k;
	first = new T[_k];
	last = new T[_k];
	cur_perm = new T[_k];
	lookup_table = new int[_n];
	std::fill_n(lookup_table, n, 0);
	full_seq = new T[_n];
	for (int i = 0; i < _n; ++i)
	{
		full_seq[i] = full_seq_sorted[i];
		//lookup_table[i] = 0;
	}
	for (int i = 0; i<_k; ++i)
	{
		first[i] = full_seq_sorted[i];
		last[i] = full_seq_sorted[_n - 1 - i];
		cur_perm[i] = full_seq_sorted[i];
		lookup_table[i]++;
	}
}

template<class T>
void PermSeq<T>::setup(int _n, int _k, T *full_seq_sorted, T *init_perm)
{
	setup(_n, _k, full_seq_sorted);
	for (int i = 0; i < k; ++i)
	{
		cur_perm[i] = init_perm[i];
		auto itr = std::lower_bound(full_seq_sorted, full_seq_sorted+_n, init_perm[i]);
		++lookup_table[std::distance(full_seq_sorted, itr)];

	}
}

template<class T>
void PermSeq<T>::setup(T *full_seq_begin, T *full_seq_end, int _k)
{
	setup(full_seq_end - full_seq_begin, _k, full_seq_begin);
}

template<class T>
void PermSeq<T>::setup(T *full_seq_begin, T *full_seq_end, T *init_perm_begin, T *init_perm_end)
{
	setup(full_seq_end - full_seq_begin, init_perm_end - init_perm_begin, full_seq_begin, init_perm_begin);
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

#endif
