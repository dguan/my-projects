#ifndef _PERMUTATION_H_
#define _PERMUTATION_H_

#include <vector>
#include <memory>
#include <algorithm>

int lexi_sn_idx(int n, int k, int perm[]);
std::vector<int> nth_perm_idx(int n, int k, int lexi_sn);

class PermIdx
{
public:
	PermIdx() : n(0), k(0), first(nullptr), last(nullptr), cur_perm(nullptr), lookup_table(nullptr) {};
	PermIdx(int _n, int _k) { setup(_n, _k); };
	PermIdx(int _n, int _k, int *init_perm) { setup(_n, _k, init_perm); };
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
