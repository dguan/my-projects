#include <gtest/gtest.h>
#include "LRU.h"

using namespace std;

TEST(LRUCacheTest01, testCachePut)
{
	LRUCache<int, string> *cache = new LRUCache<int, string>(5);
	ASSERT_TRUE(cache != nullptr);
	EXPECT_EQ(cache->Size(), 0);

	cache->Put(1, "Anna");
	EXPECT_EQ(cache->Size(), 1);

	cache->Put(2, "Brenda");
	EXPECT_EQ(cache->Size(), 2);

	cache->Put(3, "Cynthia");
	EXPECT_EQ(cache->Size(), 3);

	cache->Put(4, "Donna");
	EXPECT_EQ(cache->Size(), 4);

	cache->Put(5, "Elisa");
	EXPECT_EQ(cache->Size(), 5);

	cache->Put(6, "Flora");
	EXPECT_EQ(cache->Size(), 5);

	cache->Put(7, "Gloria");
	EXPECT_EQ(cache->Size(), 5);
}

TEST(LRUCacheTest02, testCacheDiscarding)
{
	LRUCache<int, string> *cache = new LRUCache<int, string>(5);
	ASSERT_TRUE(cache != nullptr);
	cache->Put(1, "Anna");
	cache->Put(2, "Brenda");
	cache->Put(3, "Cynthia");
	cache->Put(4, "Donna");
	cache->Put(5, "Elisa");
	
	// as I put a new item (6, "Flora"), current LRU item (1, "Anna") will be discarded,
	// the (2, "Brenda") should still be in the cache, but after Get() it will not be
	// the LRU item but will be MRU item, now LRU item will be (3, "Cynthia") 
	cache->Put(6, "Flora");
	EXPECT_FALSE(cache->Get(1).not_null);
	EXPECT_TRUE(cache->Get(2).not_null);
	EXPECT_EQ(cache->Get(2).val, "Brenda");

	// as I put in a new item (7, "Gloria"), current LRU item (3, "Cynthia") will be discarded,
	// (2, "Brenda"), (4, "Donna"), (5, "Elisa"), (6, "Flora"), and (7, "Gloria") will be in the cache.
	cache->Put(7, "Gloria");
	EXPECT_FALSE(cache->Get(1).not_null);
	EXPECT_FALSE(cache->Get(3).not_null);
	
	EXPECT_TRUE(cache->Get(2).not_null);
	EXPECT_EQ(cache->Get(2).val, "Brenda");

	EXPECT_TRUE(cache->Get(4).not_null);
	EXPECT_EQ(cache->Get(4).val, "Donna");

	EXPECT_TRUE(cache->Get(5).not_null);
	EXPECT_EQ(cache->Get(5).val, "Elisa");

	EXPECT_TRUE(cache->Get(6).not_null);
	EXPECT_EQ(cache->Get(6).val, "Flora");

	EXPECT_TRUE(cache->Get(7).not_null);
	EXPECT_EQ(cache->Get(7).val, "Gloria");
}

TEST(LRUCacheTest03, testCacheUpdating)
{
	LRUCache<int, string> *cache = new LRUCache<int, string>(5);
	ASSERT_TRUE(cache != nullptr);
	cache->Put(1, "Anna");
	cache->Put(2, "Brenda");
	cache->Put(3, "Cynthia");
	cache->Put(4, "Donna");
	cache->Put(5, "Elisa");
	
	cache->Put(1, "Adam");
	EXPECT_TRUE(cache->Get(1).not_null);
	EXPECT_EQ(cache->Get(1).val, "Adam");
	
	cache->Put(2, "Bob");
	EXPECT_TRUE(cache->Get(2).not_null);
	EXPECT_EQ(cache->Get(2).val, "Bob");

	cache->Put(1, "Ashley");
	EXPECT_TRUE(cache->Get(1).not_null);
	EXPECT_EQ(cache->Get(1).val, "Ashley");
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);  
  
    // Runs all tests using Google Test.  
    return RUN_ALL_TESTS();
}

/*

Running unit test:
  
dgu@lynx:~/TopconTest/test3$ ./TopconTest3_ut 
[==========] Running 3 tests from 3 test cases.
[----------] Global test environment set-up.
[----------] 1 test from LRUCacheTest01
[ RUN      ] LRUCacheTest01.testCachePut
[       OK ] LRUCacheTest01.testCachePut (1 ms)
[----------] 1 test from LRUCacheTest01 (1 ms total)

[----------] 1 test from LRUCacheTest02
[ RUN      ] LRUCacheTest02.testCacheDiscarding
[       OK ] LRUCacheTest02.testCacheDiscarding (0 ms)
[----------] 1 test from LRUCacheTest02 (0 ms total)

[----------] 1 test from LRUCacheTest03
[ RUN      ] LRUCacheTest03.testCacheUpdating
[       OK ] LRUCacheTest03.testCacheUpdating (0 ms)
[----------] 1 test from LRUCacheTest03 (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 3 test cases ran. (1 ms total)
[  PASSED  ] 3 tests.

*/
