This is a great example for BST and Catalan number, because the number of all BSTs could be constructed from N unique values is the N-th Catalan Number, http://en.wikipedia.org/wiki/Catalan_number, I also have a catalan number project. And, personally I think it is also a great example and comparison between normal pointers and shared_ptrs.

From statistics you can clearly see that, for shared_ptrs, only necessary nodes are created, and thses nodes are shared among shared BSTs, also, you do not have to worry about killing these nodes manually.

But for normal pointers, you have to create much more normal nodes because each BST have to keep its own copy, and also you have to kill all these nodes manually.
