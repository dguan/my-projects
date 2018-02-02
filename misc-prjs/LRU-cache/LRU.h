#ifndef _LRU_CACHE_HEADER_
#define _LRU_CACHE_HEADER_

#include <unordered_map>
#include <list>
#include <utility>
#include <stdexcept>

using namespace std;

// As I am not familiar with boost::Optional, here I define a custom 'nullable' return type
template<class VALUE> struct ret_val
{
	bool not_null;
	VALUE val;

	ret_val() : not_null(false) {};
	ret_val(bool tf, const VALUE& v)
	{
		not_null = tf;
		val = v;
	}
};
template<class VALUE> using RET_VAL = struct ret_val<VALUE>;


// The LRUCache container class
template<class KEY, class VALUE>
class LRUCache
{
public:
	LRUCache(std::size_t a_maxItems);

	inline RET_VAL<VALUE> Get(const KEY& a_key);
	inline void Put(const KEY& a_key, const VALUE& a_value);
	inline const std::size_t Size() const;

private:
	// This is the hash index of the cache, using std::list's iterator to directly
	// point to the cache item, because std::list::iterator keeps its validity
	std::unordered_map<KEY, typename std::list<std::pair<KEY, VALUE>>::iterator> m_index;

	// Using list as container because this can achieve constant accessing speed by using its iterator
	std::list<std::pair<KEY, VALUE>> m_container;

	size_t m_maxCount;
	size_t m_curCount;
};


template<class KEY, class VALUE>
inline LRUCache<KEY, VALUE>::LRUCache(size_t a_maxItems) : m_maxCount(a_maxItems), m_curCount(0)
{
}

template<class KEY, class VALUE>
inline const size_t LRUCache<KEY, VALUE>::Size() const
{
	return m_curCount;
}

// The Get() function
template<class KEY, class VALUE>
inline RET_VAL<VALUE> LRUCache<KEY, VALUE>::Get(const KEY& a_key)
{
	if (m_curCount == 0)
	{
		return RET_VAL<VALUE>(); // maybe I should throw an exception here
	}

	auto itr = m_index.find(a_key);

	if (itr != m_index.end()) // the item is in the cache
	{
		// move the old cache item into the cache's front
		m_container.push_front(move(*itr->second));

		// erase the old cache item
		m_container.erase(itr->second);

		// update the hash index to point to the cache's front
		m_index[a_key] = m_container.begin();

		// Indicating that the item is found and return its value
		return RET_VAL<VALUE>(true, m_container.front().second);
	}
	else
	{
		// The item is not in the cache, return a default RET_VAL indicating that it is not found
		return RET_VAL<VALUE>();
	}
}

// The Put() function
template<class KEY, class VALUE>
inline void LRUCache<KEY, VALUE>::Put(const KEY& a_key, const VALUE& a_value)
{
	auto itr = m_index.find(a_key);

	if (itr != m_index.end()) // The item is in the cache
	{
		m_container.erase(itr->second); // erase the old item from the cache

		m_container.emplace_front(a_key, a_value); // put the new item into the front of the cache

		itr->second = m_container.begin(); // update the hash index
	}
	else
	{
		// This cache size checking for each Put() operation could be expensive,
		// Maybe we can skip this size checking by pre-populating the cache with
		// some impossible values. So, eventually these impossible values will
		// be discarded and the cache will be filled with real values.
		if (m_curCount >= m_maxCount)
		{
			// find in the hash map the index of the item to be discarded 
			const KEY& keyRef = m_container.back().first;
			auto hashItr = m_index.find(keyRef);
			
			// The item is in the cache, but its index not in the hash map, how could this happen??????
			// Again, this checking might be unnecessary and expensive for each Put() operation
			if (hashItr == m_index.end())
			{
				throw runtime_error("Cannot find the index, this should not happen!");
			}

			// remove the index from the hash map
			m_index.erase(hashItr);

			// discard the least recently used item from the cache's back
			m_container.pop_back();

			// push the new item into the cache's front
			m_container.emplace_front(a_key, a_value);
			
			// setup the hash index
			m_index[a_key] = m_container.begin();
		}
		else
		{
			// push the new item into the front of the cache
			m_container.emplace_front(a_key, a_value);

			// setup the hash index
			m_index[a_key] = m_container.begin();

			// increment the item count
			++m_curCount;
		}
	}
}

#endif
