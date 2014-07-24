#ifndef _MY_COMBINATION_H_
#define _MY_COMBINATION_H_

#include "comb_impl.h"
#include <vector>
#include <memory>
#include <algorithm>

struct CombIdxImpl;
template<class T> struct CombSeqImpl;

class CombIdx
{
public:
	CombIdx(int _n, int _k);
	CombIdx(int _n, int _k, int *init_comb);
	bool next_comb_idx(void);
	bool prev_comb_idx(void);
	const std::vector<int>& first(void) const { return pImpl->first; };
	const std::vector<int>& last(void) const { return pImpl->last; };
	const std::vector<int>& get_cur_comb(void) const { return pImpl->cur_comb; };
	void set_cur_comb(int cur_comb[]);
	int lexi_serial_nr(int comb[]);
	std::vector<int> the_nth_comb(int lexi_sn);
	
private:
	int n;
	int k;
	std::unique_ptr<CombIdxImpl> pImpl;
};

CombIdx::CombIdx(int _n, int _k) : n(_n), k(_k)
{
	pImpl = std::unique_ptr<CombIdxImpl>(new CombIdxImpl(_n, _k));
}

CombIdx::CombIdx(int _n, int _k, int *init_comb) : n(_n), k(_k)
{
	pImpl = std::unique_ptr<CombIdxImpl>(new CombIdxImpl(_n, _k, init_comb));
}

bool CombIdx::next_comb_idx(void)
{
	if (pImpl->cur_comb[0] == n - k)
		return false;
	int cur_pos = k - 1;
	int cur_max = n - 1;
	while (true)
	{
		int cur_idx = pImpl->cur_comb[cur_pos] + 1;
		if (cur_idx <= cur_max)
		{
			while (cur_pos < k)
				pImpl->cur_comb[cur_pos++] = cur_idx++;
			return true;
		}
		else
		{
			--cur_pos;
			--cur_max;
		}
	}
}

bool CombIdx::prev_comb_idx(void)
{
	if (pImpl->cur_comb[k - 1] == k - 1)
		return false;
	int cur_pos = k - 1;
	int cur_max = n;
	while (true)
	{
		int cur_idx = pImpl->cur_comb[cur_pos] - 1;
		if (cur_pos==0 || cur_idx > pImpl->cur_comb[cur_pos - 1])
		{
			pImpl->cur_comb[cur_pos++] = cur_idx;
			while (cur_pos < k)
				pImpl->cur_comb[cur_pos++] = cur_max++;
			return true;
		}
		else
		{
			--cur_pos;
			--cur_max;
		}
	}
}

void CombIdx::set_cur_comb(int cur_comb[])
{
	for (int i = 0; i < k; ++i)
		pImpl->cur_comb[i] = cur_comb[i];
}




template<class T> 
class CombSeq
{
public:
	CombSeq(int _n, int _k, T *full_seq);
	CombSeq(int _n, int _k, T *full_seq, T *init_comb);
	bool next_comb_seq(void);
	bool prev_comb_seq(void);
	const std::vector<T>& first(void) const { return pImpl->first; };
	const std::vector<T>& last(void) const { return pImpl->last; };
	const std::vector<T>& get_cur_comb(void) const { return pImpl->cur_comb; };
	void set_cur_comb(T cur_comb[]);
	
private:
	int n;
	int k;
	std::unique_ptr<CombSeqImpl<T>> pImpl;
};

template<class T>
CombSeq<T>::CombSeq(int _n, int _k, T *full_seq) : n(_n), k(_k)
{
	pImpl = std::unique_ptr<CombSeqImpl<T>>(new CombSeqImpl<T>(_n, _k, full_seq));
}
template<class T>
CombSeq<T>::CombSeq(int _n, int _k, T *full_seq, T *init_comb) : n(_n), k(_k)
{
	pImpl = std::unique_ptr<CombSeqImpl<T>>(new CombSeqImpl<T>(_n, _k, full_seq, init_comb));
};

template<class T>
bool CombSeq<T>::next_comb_seq(void)
{
	if (pImpl->cur_comb[0] == pImpl->full_seq[n - k])
		return false;
	int cur_pos = k - 1;
	int chg_cnt = 0;
	while (true)
	{
		auto itr = std::upper_bound(pImpl->full_seq.begin(), pImpl->full_seq.end(), pImpl->cur_comb[cur_pos]);
		if (std::distance(itr, pImpl->full_seq.end()) > chg_cnt)
		{
			while (cur_pos < k)
				pImpl->cur_comb[cur_pos++] = *itr++;
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
	if (pImpl->cur_comb[k - 1] == pImpl->full_seq[k - 1])
		return false;
	int cur_pos = k - 1;
	int fill_idx = n;
	while (true)
	{
		auto itr = std::lower_bound(pImpl->full_seq.begin(), pImpl->full_seq.end(), pImpl->cur_comb[cur_pos]);
		if (cur_pos == 0 || *(itr - 1) > pImpl->cur_comb[cur_pos - 1])
		{
			pImpl->cur_comb[cur_pos++] = *(itr - 1);
			itr = pImpl->full_seq.begin() + fill_idx;
			while (cur_pos < k)
				pImpl->cur_comb[cur_pos++] = *itr++;
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
void CombSeq<T>::set_cur_comb(T cur_comb[])
{
	for (int i = 0; i < k; ++i)
		pImpl->cur_comb[i] = cur_comb[i];
}

//template CombSeq<char>::CombSeq(int, int, char *);

#endif
