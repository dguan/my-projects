This is an extended classic missionary-cannibal problem, not only find out the way to go across the river, but, because each missionary and cannibal takes different time to row the boat across the river, we would like to find out the optimal way that takes the least time to across the river.

There is a very resonable presumption that, if multiple people in the boat, the time to get to the other side is equal to the minimum time of these people.

This time is much complex than the classic missionary-cannibal problem, because that, there are more combinations than the classic problem.

For example, for the simplest condition, there're 3 missionaries and 3 cannibals, and the boat's volume is 2, in classic problem, there're at most 3 possible combinations: mc->, cc->, or c->(this one can be ommited because the cannibal need to row the boat back, so returning to the original state), mm-> or m-> are not possible because the missionaries left behind will get eaten. 

But, in this extended problem, suppose each missionary's rowing time is 1, 2, 3, and each cannibal's time is 4, 5, 6, we have at most 15 possible combinations,
m1c4, m1c5, m1c6, m2c4, m2c5, m2c6, m3c4, m3c5, m3c6, c4c5, c4c6, c5c6, c4, c5, c6(these 3 can be discarded, same reason as above).

As stated above, the solution space will increase super-exponentially and soon will be too big to process. So, I divide the problem into 3 parts,
(1) find all solutions in the solution space, to demonstrate how big the solution space could be
(2) find one optimal solution, this can speed up a lot.
(3) try to find some quasi-optimal solutions for large scale problems.


