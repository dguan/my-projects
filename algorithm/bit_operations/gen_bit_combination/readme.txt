The algorithms and implementations to generate bit combinations for integer types.

For an integer of N bits, its value can be from 0 to 2^N-1, the value can consists of 0 to N '1's, the number of integers of i '1' bits equals to choose i from N, so, the number of initegers of i '1' bits forms the Pascal triangle,

C(N,0) + C(N, 1) + ...... + C(N, N-1), C(N, N), the sum is 2^N, which is also the number of N's combination's powerset, so, the combination of bits in an integer also correspond to the combination problem, and in certain circumstances is a high performance way to generate combinations.


Only integers of i '1's are considered here, because integers of i '0's can easily be transformed into (N - i) '1's.

The 4 functions in this project has been fully tested under VS2013 and g++8.4.4, using signed and unsigned char, short, int, and long long integer types(only part of tests are included in the testing function), and all works fine, well, at least for my PC which is an intel processor which is of little endian, it should also work for other processors of any endian, but this is not tested yet.