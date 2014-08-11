This is an extended classic missionary-cannibal problem, not only find out the way to go across the river, but, because each missionary and cannibal takes different time to row the boat across the river, we would like to find out the optimal way that takes the least time to across the river.

There is a very resonable presumption that, if multiple people in the boat, the time to get to the other side is equal to the minimum time of these people.

This time is much complex than the classic missionary-cannibal problem, because that, there are more combinations than the classic problem.

For example, for the simplest condition, there're 3 missionaries and 3 cannibals, and the boat's volume is 2, in classic problem, there're at most 3 possible combinations: mc->, cc->, or c->(this one can be ommited because the cannibal need to row the boat back, so returning to the original state), mm-> or m-> are not possible because the missionaries left behind will get eaten. 

But, in this extended problem, suppose each missionary's rowing time is 1, 2, 3, and each cannibal's time is 4, 5, 6, we have at most 15 possible combinations,
m1c4, m1c5, m1c6, m2c4, m2c5, m2c6, m3c4, m3c5, m3c6, c4c5, c4c6, c5c6, c4, c5, c6(these 3 can be discarded, same reason as above).

As we have learned from the classic problem, it will need 11 runs to cross the river safely, so, the total combination space is huge! So, BFS algorithm is almost impossible, and, although DFS algorithm is much more efficient than BFS, yet, because of the scale of the complexity, althouth this algorithm can deal with any number of missionaries and cannibals, in practical, the number of missionary and cannibals are limited, especially when boat's volume is increased. My experience is not to set missionaries and cannibals's number bigger than 7 with a boat volume is 4.

One thing to note is that, I made thia program to return ALL possible solutions, trying to find how big the solution space could be. Actually I could setup a global minimal time, and eliminate lots of unnecessary solutions, this can largely increase the speed. 

Another thing to note is that, because of the scale of the complexity, when it's not practical to find the optimal solution, I could use some other algorithms to find some nearly-optimal(quasi-optimal) solution, which I am going to cover in following projects.

The output of DFS algorithm is:

******************** DFS algorithm *********************

20 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  c1<-  c1c2-> 

20 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  c1<-  c1c3-> 

22 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  c2<-  c1c2-> 

22 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  c2<-  c2c3-> 

23 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  c3<-  c1c3-> 

23 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  c3<-  c2c3-> 

24 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  c1<-  c1c3-> 

24 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  m4<-  m4c1-> 

24 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  c1<-  c1c2-> 

24 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  m4<-  m4c2-> 

25 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  m4<-  m4c3-> 

25 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  m5<-  m5c1-> 

25 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  m5<-  m5c2-> 

26 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  c2<-  c1c2-> 

26 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  m6<-  m6c1-> 

26 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  m5<-  m5c3-> 

26 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  c2<-  c2c3-> 

26 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  m6<-  m6c2-> 

27 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  c3<-  c1c3-> 

27 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  c3<-  c2c3-> 

27 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  m6<-  m6c3-> 

28 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  m4<-  m4c1-> 

28 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  m4<-  m4c2-> 

29 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  m5<-  m5c1-> 

29 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  m5<-  m5c2-> 

29 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  m4<-  m4c3-> 

30 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c2c3->  m6<-  m6c1-> 

30 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  m6<-  m6c2-> 

30 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  m5<-  m5c3-> 

31 ::  m4c1->  m4<-  c2c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c2->  m6<-  m6c3-> 

***** Total 30 DFS solutions *****

The best DFS solution is: 20 ::  c1c2->  c1<-  c1c3->  c1<-  m4m5->  m4c2<-  m4m6->  c3<-  c1c3->  c1<-  c1c2-> 


