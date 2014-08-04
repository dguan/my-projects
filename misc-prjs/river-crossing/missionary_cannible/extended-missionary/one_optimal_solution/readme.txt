This is an extended classic missionary-cannible problem, not only find out the way to go across the river, but, because each missionary and cannible takes different time to row the boat across the river, we would like to find out the optimal way that takes the least time to across the river.

There is a very resonable presumption that, if multiple people in the boat, the time to get to the other side is equal to the minimum time of these people.

This time is much complex than the classic missionary-cannible problem, because that, there are more combinations than the classic problem.

For example, for the simplest condition, there're 3 missionaries and 3 cannibles, and the boat's volume is 2, in classic problem, there're at most 3 possible combinations: mc->, cc->, or c->(this one can be ommited because the cannible need to row the boat back, so returning to the original state), mm-> or m-> are not possible because the missionaries left behind will get eaten. 

But, in this extended problem, suppose each missionary's rowing time is 1, 2, 3, and each cannible's time is 4, 5, 6, we have at most 15 possible combinations,
m1c4, m1c5, m1c6, m2c4, m2c5, m2c6, m3c4, m3c5, m3c6, c4c5, c4c6, c5c6, c4, c5, c6(these 3 can be discarded, same reason as above).

As we have learned from the classic problem, it will need 11 runs to cross the river safely, so, the total combination space is huge! So, BFS algorithm is almost impossible, and, although DFS algorithm is much more efficient than BFS, yet, because of the scale of the complexity, althouth this algorithm can deal with any number of missionaries and cannibles, in practical, the number of missionary and cannibles are limited, especially when boat's volume is increased. My experience is not to set missionaries and cannibles's number bigger than 7 with a boat volume is 4.

This program will return JUST ONE best solution, from possible multiple best-solutions. Again, BFS algorithm is much slower than DFS, and also not to try DFS on problems bigger than 7 missionaries, 7 cannbles, boat_volume = 4. 

Another thing to note is that, because of the scale of the complexity, when it's not practical to find the optimal solution, I could use some other algorithms to find some nearly-optimal(quasi-optimal) solution, which I am going to cover in following projects.



