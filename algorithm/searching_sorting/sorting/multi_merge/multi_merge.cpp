#include <iostream>
#include <vector>
#include <memory>
#include <numeric>
#include <algorithm>
#include <functional>
#include <exception>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////
//
//    Multi-Way Merge Sort
//    There are multiple data sources, each data source is sorted,
//    Try to merge all these data sources into one single sorted data.
//
/////////////////////////////////////////////////////////////////////////


template<class T> struct _data_src_descriptor
{
	T *data;
	int count;
	int cur_pos;

	_data_src_descriptor() = default;
	_data_src_descriptor(T *dt, int cnt, int cp = 0) : data(dt), count(cnt), cur_pos(cp) {};
};
template<class T> using DSD = _data_src_descriptor<T>;

template<class T> struct _merge_control_block
{
	std::vector<DSD<T>> dsds;
	int active_srcs;

	_merge_control_block() : active_srcs(0) {};
	void print_info(void);
};

template<class T> void _merge_control_block<T>::print_info(void)
{
	std::cout << "There are " << dsds.size() << " data sources, " << active_srcs << " are active." << std::endl;
	std::cout << "The info of data sources are: " << std::endl;
	for (unsigned int i = 0; i<dsds.size(); ++i)
	{
		std::cout << "Data Source [" << i << "] :" << std::endl;
		std::cout << "Length is " << dsds[i].count << ", current position is " << dsds[i].cur_pos << std::endl;
		if (dsds[i].count > 0)
			std::cout << "the first element is: " << dsds[i].data[0];
		if (dsds[i].cur_pos >= 0 && dsds[i].cur_pos < dsds[i].count)
			std::cout << ", current element is: " << dsds[i].data[dsds[i].cur_pos] << std::endl;
		std::cout << std::endl;
	}
}
template<class T> using MCB = _merge_control_block<T>;


///////////////////////////////////////////////////////////////////
//
//    Heap management using classic hand-write algorithm
//
///////////////////////////////////////////////////////////////////

template<class T> static void mm_build_heap(std::vector<DSD<T>>& dt, int data_len)
{
	int pos = data_len - 1;
	int parent = (pos - 1) / 2;

	while (parent >= 0)
	{
		int smallest = parent;
		int left = parent * 2 + 1;
		int right = parent * 2 + 2;

		if (left <= pos && dt[smallest].data[dt[smallest].cur_pos]>dt[left].data[dt[left].cur_pos])
			smallest = left;
		if (right <= pos && dt[smallest].data[dt[smallest].cur_pos]>dt[right].data[dt[right].cur_pos])
			smallest = right;

		if (smallest != parent)
		{
			std::swap(dt[parent], dt[smallest]);	// std::swap can be applied on trivial types, and DSD<scalar-types> are trivial
			parent = (parent - 1) / 2;
		}
		else
			break;
	}
}

template<class T> static void mm_update_heap(std::vector<DSD<T>>& dsds, int count)
{
	int parent = 0;
	while (parent <= count / 2)
	{
		int smallest = parent;
		int left = parent * 2 + 1;
		int right = parent * 2 + 2;

		if (left<count && dsds[smallest].data[dsds[smallest].cur_pos]>dsds[left].data[dsds[left].cur_pos])
			smallest = left;
		if (right<count && dsds[smallest].data[dsds[smallest].cur_pos]>dsds[right].data[dsds[right].cur_pos])
			smallest = right;

		if (smallest != parent)
		{
			std::swap(dsds[parent], dsds[smallest]);
			parent = smallest;
		}
		else
			break;
	}
}

template<class T> void multi_merge(MCB<T> *p_mcb, T *output)
{
	if (p_mcb == nullptr || output == nullptr)
		return;
	T *cur_insertion_pos = output;

	while (p_mcb->active_srcs > 1)
	{
		DSD<T>& cur_dsd = p_mcb->dsds[p_mcb->active_srcs - 1];
		std::swap(cur_dsd, p_mcb->dsds[0]);

		mm_update_heap(p_mcb->dsds, p_mcb->active_srcs - 1);
		//p_mcb->print_info();

		T next_min = p_mcb->dsds[0].data[p_mcb->dsds[0].cur_pos];
		bool cur_data_source_finished = false;
		while (cur_dsd.data[cur_dsd.cur_pos] <= next_min)
		{
			*cur_insertion_pos++ = cur_dsd.data[cur_dsd.cur_pos++];
			if (cur_dsd.cur_pos >= cur_dsd.count)
			{
				cur_data_source_finished = true;
				break;
			}
		}
		if (cur_data_source_finished)
			--p_mcb->active_srcs;
		else
			mm_build_heap(p_mcb->dsds, p_mcb->active_srcs);
	}
	if (p_mcb->active_srcs == 1)
	{
		while (p_mcb->dsds[0].cur_pos < p_mcb->dsds[0].count)
			*cur_insertion_pos++ = p_mcb->dsds[0].data[p_mcb->dsds[0].cur_pos++];
		p_mcb->active_srcs = 0;
	}
}

template<class T> std::unique_ptr<MCB<T>> init_mcb(T *src[], int src_len[], int cnt)
{
	std::unique_ptr<MCB<T>> p(new(std::nothrow) MCB<T>);
	if (p)
	{
		for (int i = 0; i<cnt; ++i)
		{
			if (src_len[i] > 0)
			{
				p->dsds.emplace_back(src[i], src_len[i]);
				mm_build_heap(p->dsds, p->dsds.size());
				++p->active_srcs;
			}
		}
	}
	return p;
}

///////////////////////////////////////////////////////////////////
//
//    Heap management using mordern STL algorithm library
//
///////////////////////////////////////////////////////////////////

// It's also convenient to use a lambda function. [&](const DSD<T>& a, const DSD<T>& b) { return a.data[a.cur_pos] > b.data[b.cur_pos]; }
template<class T> bool dsd_small_top_heap_cmp(const DSD<T>& a, const DSD<T>& b)
{
	return a.data[a.cur_pos] > b.data[b.cur_pos];
}

template<class T> void multi_merge_stl(MCB<T> *p_mcb, T *output)
{
	if (p_mcb == nullptr || output == nullptr)
		return;
	T *cur_insertion_pos = output;

	while (p_mcb->active_srcs > 1)
	{
		std::pop_heap(p_mcb->dsds.begin(), p_mcb->dsds.begin() + p_mcb->active_srcs, [&](const DSD<T>& a, const DSD<T>& b) { return a.data[a.cur_pos] > b.data[b.cur_pos]; });
		//p_mcb->print_info();

		DSD<T>& cur_dsd = p_mcb->dsds[p_mcb->active_srcs-1];
		T next_min = p_mcb->dsds[0].data[p_mcb->dsds[0].cur_pos];
		bool cur_data_source_finished = false;
		while (cur_dsd.data[cur_dsd.cur_pos] <= next_min)
		{
			*cur_insertion_pos++ = cur_dsd.data[cur_dsd.cur_pos++];
			if (cur_dsd.cur_pos >= cur_dsd.count)
			{
				cur_data_source_finished = true;
				break;
			}
		}
		if (cur_data_source_finished)
			--p_mcb->active_srcs;
		else
			std::push_heap(p_mcb->dsds.begin(), p_mcb->dsds.begin() + p_mcb->active_srcs, &dsd_small_top_heap_cmp<T>);
	}
	if (p_mcb->active_srcs == 1)
	{
		while (p_mcb->dsds[0].cur_pos < p_mcb->dsds[0].count)
			*cur_insertion_pos++ = p_mcb->dsds[0].data[p_mcb->dsds[0].cur_pos++];
		p_mcb->active_srcs = 0;
	}
}

template<class T> std::unique_ptr<MCB<T>> init_mcb_stl(T *src[], int src_len[], int cnt)
{
	std::unique_ptr<MCB<T>> p(new(std::nothrow) MCB<T>);
	if (p)
	{
		for (int i = 0; i<cnt; ++i)
		{
			if (src_len[i] > 0)
			{
				p->dsds.emplace_back(src[i], src_len[i]);
				std::make_heap(p->dsds.begin(), p->dsds.end(), &dsd_small_top_heap_cmp<T>);
				++p->active_srcs;
			}
		}
	}
	return p;
}


int main(void)
{
	int data_1[] = { 11, 31, 51, 71, 91 };
	int data_2[] = { 22, 42, 62, 82 };
	int data_3[] = { 33, 73, 93 };
	int data_4[] = { 4, 24, 54, 84, 104, 114, 144 };
	int data_5[] = { 15, 25, 35, 45, 55, 65, 75, 85, 95 };

	int *data_sources[] = { data_1, data_2, data_3, data_4, data_5 };
	int data_src_lens[] = { sizeof(data_1) / sizeof(int), sizeof(data_2) / sizeof(int), sizeof(data_3) / sizeof(int), sizeof(data_4) / sizeof(int), sizeof(data_5) / sizeof(int) };

	int total_data_len = std::accumulate(std::begin(data_src_lens), std::end(data_src_lens), 0);

	//std::unique_ptr<int[]> merged_data = std::make_unique<int[]>(total_data_len);// what a pity g++ 8.4 does not have make_unique
	std::unique_ptr<int[]> merged_data(new(std::nothrow) int[total_data_len]);
	if (!merged_data)
		throw std::runtime_error("Cannot allocate memory for merged data");

	// The STL heap management version
	std::unique_ptr<MCB<int>> p = init_mcb_stl(data_sources, data_src_lens, sizeof(data_sources) / sizeof(data_sources[0]));
	if (!p)
		throw std::runtime_error("Failed to initialize MCB");
	multi_merge_stl(p.get(), merged_data.get());
	std::cout << "multi_merge_stl result is: " << std::endl;
	for (int i = 0; i<total_data_len; ++i)
		std::cout << "[" << merged_data[i] << "] ";
	std::cout << std::endl;

	// The hand-write heap management version
	std::unique_ptr<MCB<int>> q = init_mcb(data_sources, data_src_lens, sizeof(data_sources) / sizeof(data_sources[0]));
	if (!q)
		throw std::runtime_error("Failed to initialize MCB");
	multi_merge(q.get(), merged_data.get());
	std::cout << "multi_merge result is: " << std::endl;
	for (int i = 0; i<total_data_len; ++i)
		std::cout << "[" << merged_data[i] << "] ";
	std::cout << std::endl;

	return 0;
}

