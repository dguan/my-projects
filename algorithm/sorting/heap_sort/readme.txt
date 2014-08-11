Implementation of non-recursive heap sort algorithm and performance bentch marking with STL algorithm.

The bentch marking is like this (compiled with gcc, with -O3 optimization):

This implementation      STL make_heap and sort_heap      std::sort
      247 ms                    136 ms                     72 ms
      253 ms                    139 ms                     72 ms
      248 ms                    138 ms                     72 ms
      
The STL algorithm is about 50% faster than this algorithm. Yet, this algorithm can do both big-top and small-top heap sorting,
so, there are some logics to determine what kind of top we're using, and this slowes it down a lot. Also, I did not optimized
this algorithm too much, it is only a pretty basic implementation.
