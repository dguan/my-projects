Algorithm to seperate an integer into all non-repetitive parts, which is called number partition. 

The WIKI page about partition:
http://en.wikipedia.org/wiki/Partition_(number_theory)

Sequence partitioning is to find the ways to partition a sequence into sub-parts, it is different from number partitioning, yet quite related to the number partitioning, especially if I partition on the sequence's length.

for example, I have a sequence(string) "abcd", its partitions are:
a, b, c, d
a, b, cd
a, bc, d
a, bcd
ab, c, d
ab, cd
abc, d
abcd
 
If I partition on "abcd"'s length which is 4, the partition are:
1, 1, 1, 1  -->  a, b, c, d
1, 1, 2     -->  a, b, cd
1, 2, 1     -->  a, bc, d
1, 3        -->  a, bcd
2, 1, 1     -->  ab, c, d
2, 2        -->  ab, cd
3, 1        -->  abc, d
4           -->  abcd

As a comparison, the integer number 4's partitions are:
1, 1, 1, 1
1, 1, 2
1, 3
2, 2
4

So, partition on sequence's length is equal to number partition 'with duplications', these duplications reflects the position(sequence) property of the sequence.


An interesting thing is that, given a sequence of length N, the total number of its [1, 2] partitions equals Fibonacci(N),

Fibonacci numbers: Fib(0) = 1, Fib(1) = 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ......

The number of string("a") 's partition is 1; [a]
The number of string("ab")'s partition is 2; [a, b], [ab]
The number of string("abc") 's partition is 3; [a, b, c], [a, bc], [ab, c]
The number of string("abcd")'s partition is 5; [a, b, c, d], [a, b, cd], [a, bc, d], [ab, c, d], [ab, cd]
......
