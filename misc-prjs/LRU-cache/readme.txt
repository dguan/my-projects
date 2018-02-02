One of my personal favorite, this problem was from an online testing, I provided the pseudo code in 30 minutes, and was later asked to provide a fully working code and its unit test, I finished the task and secured a full time job. :)


The question:

Please implement a container to act as a least-recently-used (LRU) cache. The requirements are:
1) A cache item consists of a KEY and a VALUE.
2) When instantiated, the cache has a fixed limit on the number of items that can be stored.
3) Cache items can be added using a 'Put' method and retrieved using a 'Get' method.
4) The cache cannot store duplicate keys.  If a 'Put' operation is performed using a KEY that already exists in the cache, the VALUE in the cache is updated.
5) Both 'Put' and 'Get' operations will make the item the most recently used item.
6) If a new item is inserted when the cache is full, then the least recently used item is evicted prior to inserting the new item.
7) Both 'Put' and 'Get' operations should be fast, even when the cache is very large (i.e. faster than O(N)).
8) You do not need to make the container thread-safe.
Given these requirements, please implement the following class, adding to the definition as needed:
template<class KEY, class VALUE>
class LRUCache
{
public:
    LRUCache(std::size_t a_maxItems);
    inline boost::optional<VALUE> Get(const KEY& a_key)
    {
    }
    inline void Put(const KEY& a_key, const VALUE& a_value)
    {
    }
    inline const size_t Size() const
    {
    }
private:
};


