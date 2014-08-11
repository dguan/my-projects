#ifndef _MY_PERM_IMPL_H_
#define _MY_PERM_IMPL_H_

#include <vector>
#include <algorithm>

struct PermIdxImpl
{
	std::vector<int> first;
	std::vector<int> last;
	std::vector<int> cur_perm;
	std::vector<int> lookup_table;

	PermIdxImpl(int _n, int _k);
	PermIdxImpl(int _n, int _k, int *_init_perm);
};

PermIdxImpl::PermIdxImpl(int _n, int _k)
{
	for (int i = 0; i < _n; ++i)
		lookup_table.push_back(0);
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(i);
		last.push_back(_n - 1 - i);
		cur_perm.push_back(i);
		lookup_table[i] = 1;
	}
}

PermIdxImpl::PermIdxImpl(int _n, int _k, int *_init_perm)
{
	for (int i = 0; i < _n; ++i)
		lookup_table.push_back(0);
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(i);
		last.push_back(_n - 1 - i);
		cur_perm.push_back(_init_perm[i]);
		lookup_table[_init_perm[i]] = 1;
	}
}



template<class T>
struct PermSeqImpl
{
	std::vector<T> first;
	std::vector<T> last;
	std::vector<T> cur_perm;
	std::vector<T> full_seq;
	std::vector<int> lookup_table;

	PermSeqImpl(int _n, int _k, T *_full_seq);
	PermSeqImpl(int _n, int _k, T *_full_seq, T *_init_perm);
};


template<class T>
PermSeqImpl<T>::PermSeqImpl(int _n, int _k, T *_full_seq)
{
	for (int i = 0; i < _n; ++i)
	{
		full_seq.push_back(_full_seq[i]);
		lookup_table.push_back(0);
	}
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(_full_seq[i]);
		last.push_back(_full_seq[_n - 1 - i]);
		cur_perm.push_back(_full_seq[i]);
		lookup_table[i] = 1;
	}
}

template<class T>
PermSeqImpl<T>::PermSeqImpl(int _n, int _k, T *_full_seq, T *_init_perm)
{
	for (int i = 0; i < _n; ++i)
	{
		full_seq.push_back(_full_seq[i]);
		lookup_table.push_back(0);
	}
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(i);
		last.push_back(_n - 1 - i);
		cur_perm.push_back(_init_perm[i]);

		auto itr = std::lower_bound(full_seq.begin(), full_seq.end(), _init_perm[i]);
		lookup_table[std::distance(full_seq.begin(), itr)] = 1;
	}
}

//template PermSeqImpl<char>::CombSeqImpl(int, int, char *);

#endif
