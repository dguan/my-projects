#ifndef _MY_PERMUTATION_H_
#define _MY_PERMUTATION_H_

#include "perm_impl.h"
#include <vector>
#include <memory>

struct PermIdxImpl;
template<class T> struct PermSeqImpl;

class PermIdx
{
public:
	PermIdx(int n, int k);
	PermIdx(int n, int k, int *init_perm);
	bool next_perm_idx(void);
	bool prev_perm_idx(void);
	const std::vector<int>& first(void) const { return pImpl->first; };
	const std::vector<int>& last(void) const { return pImpl->last; };
	const std::vector<int>& get_cur_perm(void) const { return pImpl->cur_perm; };
	void set_cur_perm(int cur_perm[]);
	int lexi_serial_nr(int perm[]);
	std::vector<int> the_nth_perm(int lexi_sn);

private:
	int n;
	int k;
	std::unique_ptr<PermIdxImpl> pImpl;
};

PermIdx::PermIdx(int _n, int _k) : n(_n), k(_k)
{
	pImpl = std::unique_ptr<PermIdxImpl>(new PermIdxImpl(_n, _k));
}

PermIdx::PermIdx(int _n, int _k, int *init_perm) : n(_n), k(_k)
{
	pImpl = std::move(std::unique_ptr<PermIdxImpl>(new PermIdxImpl(_n, _k, init_perm)));
}

bool PermIdx::next_perm_idx(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int cur_idx = pImpl->cur_perm[cur_pos];
		pImpl->lookup_table[cur_idx++] = 0;
		while (cur_idx < n)
		{
			if (!pImpl->lookup_table[cur_idx])
				break;
			++cur_idx;
		}
		if (cur_idx < n)
		{
			pImpl->lookup_table[pImpl->cur_perm[cur_pos]] = 0;
			pImpl->cur_perm[cur_pos++] = cur_idx;
			pImpl->lookup_table[cur_idx] = 1;
			int lookup_table_idx = 0;
			while (cur_pos < k)
			{
				while (pImpl->lookup_table[lookup_table_idx])
					++lookup_table_idx;
				pImpl->cur_perm[cur_pos++] = lookup_table_idx;
				pImpl->lookup_table[lookup_table_idx++] = 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 1; i <= k; ++i)
					pImpl->lookup_table[n-i] = 1;
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
		int cur_idx = pImpl->cur_perm[cur_pos];
		pImpl->lookup_table[cur_idx--] = 0;
		while (cur_idx >= 0)
		{
			if (!pImpl->lookup_table[cur_idx])
				break;
			--cur_idx;
		}
		if (cur_idx >= 0)
		{
			pImpl->lookup_table[pImpl->cur_perm[cur_pos]] = 0;
			pImpl->cur_perm[cur_pos++] = cur_idx;
			pImpl->lookup_table[cur_idx] = 1;
			int lookup_table_idx = n - 1;
			while (cur_pos < k)
			{
				while (pImpl->lookup_table[lookup_table_idx])
					--lookup_table_idx;
				pImpl->cur_perm[cur_pos++] = lookup_table_idx;
				pImpl->lookup_table[lookup_table_idx--] = 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 0; i < k; ++i)
					pImpl->lookup_table[i] = 1;
				return false;
			}
		}
	}
}

template<class T>
class PermSeq
{
public:
	PermSeq(int _n, int _k, T *full_seq);
	PermSeq(int _n, int _k, T *full_seq, T *init_perm);
	bool next_perm_seq(void);
	bool prev_perm_seq(void);
	const std::vector<T>& first(void) const { return pImpl->first; };
	const std::vector<T>& last(void) const { return pImpl->last; };
	const std::vector<T>& get_cur_perm(void) const { return pImpl->cur_perm; };
	void set_cur_perm(T cur_perm[]);

private:
	int n;
	int k;
	std::unique_ptr<PermSeqImpl<T>> pImpl;
};

template<class T>
PermSeq<T>::PermSeq(int _n, int _k, T *full_seq) : n(_n), k(_k)
{
	pImpl = std::unique_ptr<PermSeqImpl<T>>(new PermSeqImpl<T>(_n, _k, full_seq));
}

template<class T>
PermSeq<T>::PermSeq(int _n, int _k, T *full_seq, T *init_perm) : n(_n), k(_k)
{
	pImpl = std::unique_ptr<PermSeqImpl<T>>(new PermSeqImpl<T>(_n, _k, full_seq, init_perm));
}

template<class T>
bool PermSeq<T>::next_perm_seq(void)
{
	int cur_pos = k - 1;
	while (true)
	{
		int table_idx = std::distance(pImpl->full_seq.begin(), std::upper_bound(pImpl->full_seq.begin(), pImpl->full_seq.end(), pImpl->cur_perm[cur_pos]));
		pImpl->lookup_table[table_idx - 1] = 0;
		while (table_idx < n)
		{
			if (!pImpl->lookup_table[table_idx])
				break;
			++table_idx;
		}
		if (table_idx < n)
		{
			pImpl->cur_perm[cur_pos++] = pImpl->full_seq[table_idx];
			pImpl->lookup_table[table_idx] = 1;
			table_idx = 0;
			while (cur_pos < k)
			{
				while (pImpl->lookup_table[table_idx])
					++table_idx;
				pImpl->cur_perm[cur_pos++] = pImpl->full_seq[table_idx];
				pImpl->lookup_table[table_idx++] = 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 1; i <= k; ++i)
					pImpl->lookup_table[n-i] = 1;
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
		int table_idx = std::distance(pImpl->full_seq.begin(), std::lower_bound(pImpl->full_seq.begin(), pImpl->full_seq.end(), pImpl->cur_perm[cur_pos]));
		pImpl->lookup_table[table_idx--] = 0;
		while (table_idx >= 0)
		{
			if (!pImpl->lookup_table[table_idx])
				break;
			--table_idx;
		}
		if (table_idx >= 0)
		{
			pImpl->cur_perm[cur_pos++] = pImpl->full_seq[table_idx];
			pImpl->lookup_table[table_idx] = 1;
			table_idx = n - 1;
			while (cur_pos < k)
			{
				while (pImpl->lookup_table[table_idx])
					--table_idx;
				pImpl->cur_perm[cur_pos++] = pImpl->full_seq[table_idx];
				pImpl->lookup_table[table_idx--] = 1;
			}
			return true;
		}
		else
		{
			--cur_pos;
			if (cur_pos < 0)
			{
				for (int i = 0; i < k; ++i)
					pImpl->lookup_table[i] = 1;
				return false;
			}
		}
	}
}

#endif
