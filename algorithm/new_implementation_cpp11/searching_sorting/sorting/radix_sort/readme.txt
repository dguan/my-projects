Implementation of Radix Sort algorithm, and the performance comparison with std::sort. Unfortunately, this code cannot compile in Visual Studio 2013, because Visual Studio 2013 does not support 'constexpr' keyword. It's only tested on Linux systems with g++ 8.4.


The performance comparison with std::sort conditions:

Intel 3.0GHz processor with 4GB memory
Ubuntu14.04 with g++ 8.4.2, with -O3 optimization as shown in the Makefile.



performance comparison with std::sort on 1 million random integers:

TESTS    radix_sort    std::sort
  1        17 ms          71
  2        17 ms          71
  3        17 ms          71

performance comparison with std::sort on 10 million random integers:

TESTS    radix_sort    std::sort
  1        163 ms        789
  2        163 ms        788
  3        162 ms        788


The performance of radix_sort is nice.
