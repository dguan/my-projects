Partiton of a set -- Wikipedia webpage: http://en.wikipedia.org/wiki/Partition_of_a_set

The total number of partitions of an n-element set is the Bell number Bn. The first several Bell numbers are B0 = 1, B1 = 1, B2 = 2, B3 = 5, B4 = 15, B5 = 52, and B6 = 203. 

The Bell numbers may also be computed using the Bell triangle in which the first value in each row is copied from the end of the previous row, and subsequent values are computed by adding the two numbers to the left and above left of each position. The Bell numbers are repeated along both sides of this triangle. The numbers within the triangle count partitions in which a given element is the largest singleton.

    1
    1   2
    2   3   5
    5   7   10  15
    15  20  27  37  52

The number of partitions of an n-element set into exactly k nonempty parts is the Stirling number of the second kind S(n, k).

The number of noncrossing partitions of an n-element set is the Catalan number Cn, 

This project only implemented 2 recursive algoithms, I have tried to implement non-recursive using sequence partitioning algorithms but failed. It seems I need to find the ordering first, will try it later......