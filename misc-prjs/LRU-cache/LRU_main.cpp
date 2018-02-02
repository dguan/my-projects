#include <iostream>
#include <string>

#include "LRU.h"

using namespace std;

template<class VALUE>
ostream& operator<<(ostream& os, const RET_VAL<VALUE>& retVal)
{
	if(retVal.not_null)
		os << "Data not null and is: " << retVal.val;
	else
		os << "Data null";
	
	return os;
}

int main()
{
	LRUCache<int, string> lruCache(5);
	
	lruCache.Put(1, "Anna");
	lruCache.Put(2, "Brenda");
	lruCache.Put(3, "Cynthia");

	cout << "Currently there are " << lruCache.Size() << " items in the cache" << endl;
	
	// after this call, key 3 is still the Most Recently Used item and key 1 still the lRU item
	int key = 3;
	RET_VAL<string> ret = lruCache.Get(key);
	cout << "Key " << key << ": " << ret << endl;
	
	lruCache.Put(4, "Donna");
	lruCache.Put(5, "Elisa");
	lruCache.Put(6, "Flora");
	lruCache.Put(7, "Gloria");

	// As the maxSize is set to 5, so after above Put(), the LRU items
	// 1 and 2(Anna and Brenda) will be discarded and not in the cache
	key = 1;
	ret = lruCache.Get(key);
	cout << "Key " << key << ": " << ret << endl;

	key = 2;
	ret = lruCache.Get(key);
	cout << "Key " << key << ": " << ret << endl;
	
	// After this Put(), key 4 should be still in the cache,
	// but the value should be updated from "Donna" to "David"
	key = 4;
	ret = lruCache.Get(key);
	cout << "Key " << key << ": " << ret << endl;
	lruCache.Put(key, "David");
	ret = lruCache.Get(key);
	cout << "Key " << key << ": " << ret << endl;
	
	// After these 2 Put(), the key 3 and 5(Cynthia and Elisa) will be discarded,
	// as for key 4(Donna/David), because key 4 has been Get()/Put(), so it
	// became the MRU and will stay in the cache
	lruCache.Put(1, "Adam");
	lruCache.Put(2, "Bob");
	
	// test if Cynthia and Elisa are in the cache
	key = 3;
	ret = lruCache.Get(key);
	cout << "Key " << key << ": " << ret << endl;
	key = 5;
	ret = lruCache.Get(key);
	cout << "Key " << key << ": " << ret << endl;
	
/*

The output of the above tests are as follows:
 
dgu@lynx:~/TopconTest/test3$ ./TopconTest3 
Currently there are 3 items in the cache
Key 3: Data not null and is: Cynthia
Key 1: Data null
Key 2: Data null
Key 4: Data not null and is: Donna
Key 4: Data not null and is: David
Key 3: Data null
Key 5: Data null

The output are as expected, my compiler is(as from 'g++ --version'):
g++ (Ubuntu 5.4.0-6ubuntu1~16.04.2) 5.4.0 20160609
  
*/

	return 0;
}

