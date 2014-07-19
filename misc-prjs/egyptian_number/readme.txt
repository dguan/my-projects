The applicatoin to solve a mathmatical problem, find out all Egyptian Numbers.

Egyptian Numbers are the set of integers (I1, I2, ..., In) who satisfies the property that 1/I1 + 1/I2 + ... + 1/In = 1.

There are lots of egyptian numbers possible, actually there are too many, so that we only can find the egyptian numbers in a range(say, the maximun number no greater than 100), and the length of the integer set(how many numbers in the egyptian number set).

Note that, when the maximum number, and especially, the length of the integer set, increases, the total number of egyptian numbers and computational complexity will increase dramatically. So, it is not recommended to set the set's length longer than 8.

And also, be ware of the multiplication overflow, so I used a functoin find_gcd to find the greatest common divisor to do some simplification, yet, the best way is to use longer ingeters, like int64_t.

 
