Implementation of non-recursive, stable merge sort algorithm. Also the performance comparison with recursive algorithm, STL std::inplace_merge,  and STL std::sort.

The performance of this merge sort is quite satisfactory, for a 1 million random integer data set, performance are as follow:

Testing environment:
Intel 3.1GHz CPU, 4GB memory
Ubuntu 14.04, g++ 8.4.2, build with -O3 optimization

TESTS    merge_sort    std::sort    merge_sort_recursive    merge_sort_inplace
  1        94 ms           72               133                   114
  2        94 ms           72               133                   115
  3        94 ms           72               133                   114

The merge_sort's performance is comparable with the famous std::sort, and, it has a definite advantage over std::sort, that this merge_sort is stable.
