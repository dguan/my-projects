#ifndef _MY_COMB_IMPL_H_
#define _MY_COMB_IMPL_H_

#include <vector>

struct CombIdxImpl
{
	std::vector<int> first;
	std::vector<int> last;
	std::vector<int> cur_comb;

	CombIdxImpl(int _n, int _k);
	CombIdxImpl(int _n, int _k, int *_init_comb);
};

CombIdxImpl::CombIdxImpl(int _n, int _k)
{
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(i);
		last.push_back(_n - _k + i);
		cur_comb.push_back(i);
	}
}

CombIdxImpl::CombIdxImpl(int _n, int _k, int *_init_comb)
{
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(i);
		last.push_back(_n - _k + i);
		cur_comb.push_back(_init_comb[i]);
	}
}



template<class T>
struct CombSeqImpl
{
	std::vector<T> first;
	std::vector<T> last;
	std::vector<T> cur_comb;
	std::vector<T> full_seq;

	CombSeqImpl(int _n, int _k, T *_full_seq);
	CombSeqImpl(int _n, int _k, T *_full_seq, T *_init_comb);
};

template<class T>
CombSeqImpl<T>::CombSeqImpl(int _n, int _k, T *_full_seq)
{
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(_full_seq[i]);
		last.push_back(_full_seq[_n - _k + i]);
		cur_comb.push_back(_full_seq[i]);
	}
	for (int i = 0; i<_n; ++i)
		full_seq.push_back(_full_seq[i]);
}

template<class T>
CombSeqImpl<T>::CombSeqImpl(int _n, int _k, T *_full_seq, T *_init_comb)
{
	for (int i = 0; i<_k; ++i)
	{
		first.push_back(i);
		last.push_back(_n - _k + i);
		cur_comb.push_back(_init_comb[i]);
	}
	for (int i = 0; i<_n; ++i)
		full_seq.push_back(_full_seq[i]);
}

//template CombSeqImpl<char>::CombSeqImpl(int, int, char *);

#endif
