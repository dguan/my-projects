#ifndef _COMBINATION_H_
#define _COMBINATION_H_

#include <vector>
#include <memory>
#include <algorithm>

int lexi_sn_idx(int n, int k, int comb[]);
std::vector<int> nth_comb_idx(int lexi_sn);


class CombIdx
{
public:
	CombIdx() : n(0), k(0), first(nullptr), last(nullptr), cur_comb(nullptr) {};
	CombIdx(int _n, int _k) { setup(_n, _k); };
	CombIdx(int _n, int _k, int *init_comb) { setup(_n, _k, init_comb); };
	~CombIdx() { delete[] first; delete[] last; delete[] cur_comb; };

	void reset(void);
	void setup(int _n, int _k);
	void setup(int _n, int _k, int *init_comb);
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

void CombIdx::setup(int _n, int _k, int *init_comb)
{
	setup(_n, _k);
	for (int i = 0; i < k; ++i)
		cur_comb[i] = init_comb[i];
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
	CombSeq(int _n, int _k, T *full_seq_sorted) { setup(_n, _k, full_seq_sorted); };
	CombSeq(int _n, int _k, T *full_seq_sorted, T *init_comb) { setup(_n, _k, full_seq_sorted, init_comb); };
	CombSeq(T *full_seq_begin, T *full_seq_end, int _k) { setup(full_seq_begin, full_seq_end, _k); };
	CombSeq(T *full_seq_begin, T *full_seq_end, T *init_comb_begin, T *init_comb_end) { setup(full_seq_begin, full_seq_end, init_comb_begin, init_comb_end); };
	~CombSeq() { delete[] first; delete[] last; delete[] cur_comb; delete[] full_seq; }
	void reset(void);
	void setup(int _n, int _k, T *full_seq_sorted);
	void setup(int _n, int _k, T *full_seq_sorted, T *init_comb);
	void setup(T *full_seq_begin, T *full_seq_end, int _k);
	void setup(T *full_seq_begin, T *full_seq_end, T *init_comb_begin, T *init_comb_end);
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
void CombSeq<T>::setup(int _n, int _k, T *full_seq_sorted)
{
	n = _n;
	k = _k;
	first = new T[_k];
	last = new T[_k];
	cur_comb = new T[_k];
	full_seq = new T[_n];

	for (int i = 0; i<_n; ++i)
		full_seq[i] = full_seq_sorted[i];

	for (int i = 0; i<_k; ++i)
	{
		first[i] = full_seq_sorted[i];
		last[i] = full_seq_sorted[_n - _k + i];
		cur_comb[i] = full_seq_sorted[i];
	}
}

template<class T>
void CombSeq<T>::setup(int _n, int _k, T *full_seq_sorted, T *init_comb)
{
	setup(_n, _k, full_seq_sorted);
	for (int i = 0; i < k; ++i)
		cur_comb[i] = init_comb[i];
};

template<class T>
void CombSeq<T>::setup(T *full_seq_begin, T *full_seq_end, int _k)
{
	setup(full_seq_end - full_seq_begin, _k, full_seq_begin);

}
template<class T>
void CombSeq<T>::setup(T *full_seq_begin, T *full_seq_end, T *init_comb_begin, T *init_comb_end)
{
	setup(full_seq_end - full_seq_begin, init_comb_end - init_comb_begin, full_seq_begin, init_comb_begin);
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