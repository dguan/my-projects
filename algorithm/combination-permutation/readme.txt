This is various kinds of combination and permution algorithm's implementation, from recursive algorithm and its basic non-recursive implementation to more efficient non-recursive implementations.

Some thing to remark:

next_comb and next_perm functions:
    can to combine K from N, and permutate K from N. especially, STL library also has a next_permutation function, it's freakingly fast than my next_perm, under certain circumstances, it can be more than 10 times faster. But, my next_perm can do permutate K from N, while STL next_permutation can only do permutate N from N. There's no counter part of next_comb in STL library.

get_multi_comb_cnt and get_multi_comb_idx:
    can combine from multiple groups either by count or by index. This can be very useful dealing with complex combinational problems. For example, get_multi_comb_cnt can be used in classical missionary and cannible problem. get_multi_comb_idx can be used in extended river-crossing with a touch problem.

