This missionaries and cannibals river-crossing problem is a classical AI problem. The original problem statement is like this:

On one bank of a river are three missionaries and three cannibals.There is one boat available that can hold up to two people and that they would like to use to cross the river.If the cannibals ever outnumber the missionaries on either of the river???¡¥s banks, the missionaries will get eaten.
How can the boat be used to safely carry all the missionaries and cannibals across the river ?

This solution is an improvement over the original problem, it can deal with arbitrary number of missionaries and cannibals. In dealing with this problem, there are some points to be pointed out:

(1) There is no solution when boat's capacity is 1, the reason is very obvious.

(2) There are solutions for Missionaries = Cannibles = 3 and boat's capacity = 2.

(3) There is no solution when M=C=4 and boat's capacity=2.

(4) There are solutions when M=C=4 or 5, and the boat's capacity increased to 3.

(5) There is no solution when M=C=6, and the boat's capacity is still 3. Actually, it can be mathematically or logically proved that, when M=C=X, boat's capacity is Y(N=2 or 3), there's no solution when Y*2 <= X.

(6) If the number os missionaries is bigger than the number of cannibals, even only by 1, say, 100 missionaries and 99 cannibals, there is always solutions even when the boat's capacity is only 2.

(7) There will always be solutions when the boat's capacity is 4, the reason is also very obvious that, when the boat's capacity is 4, each time the boat can move 2 missionaries and 2 cannibals to the othere side, then one missionary and one cannibal come back to start over.

(8) There are optimal solutions which takes the least possible moves to bring all people to the other side. The optimal solution can be found out by using BFS algorithm, which is the classicla M3C3 problem. *** BUT ***, when dealing with large number of Ms and Cs(M==C), the BFS algorithm is very slow and costs too much memory space to hold temporary results. In my example, when dealing with 100 missionaries and 100 cannibals, BFS algorithm is thousands times slower than the contrast DFS algorithm.

(9) Usually, DFS algorithm cannot necessarily find the optimal solution, but in this algorithm, I played some tricks in scheduling that, by selecting the combinations that could take as many as possible people to the other side first, and by selecting the combinations that needs least people to be back, my DFS algorithm always find the optimal solution first, much more faster and much more space economic.
