This is a Google programming test 2003 problem. I first saw this problem in 2009 from a participator's blog, http://blog.csdn.net/Ackarlix/archive/2007/08/30/1764739.aspx, and was not impressed with the  triple loop solution which can only give out total number of the solutions without being able to print out the solutions. So I developed this recursive algorithm which can not only print out all solutions(if any), but also can deal with variant length inputs, http://blog.csdn.net/erdangjiade/article/details/4321146.

The first version was written in C++98, when dealingthe arbitrary input, and the performance is not very good. This is the new version in C++11, by using C++11 new features such as hash-map, hash-set, threading, etc., not only is more powerful, but a lot more performant in dealing with big scale time complexity.


And also, I developed a Python script which used the same algorithm as C++, which is quite fun to find out some magic number patterns. There's a more detailed desctiption in the Python script's usage.
 