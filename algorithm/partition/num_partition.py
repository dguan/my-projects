# -*- coding: utf-8 -*-
"""
Created on Fri Jun 13 15:35:33 2014

@author: dgu
"""

# or sequence partition
def num_part_dup(n):
    results = []
    if n==0:
        results.append([])
    else:
        for x in range(1, n+1):
            temp = num_part_dup(n-x)
            for d in temp:
                results.append([x]+d)
    return results

# or sequence partition count
def num_part_dup_cnt(n):
    cnt = 0
    if not n:
        return 1
    else:
        for x in range(1, n+1):
            cnt += num_part_dup_cnt(n-x)
    return cnt

# number partition
def num_part_nodup(n, min_div):
    results = []
    if n==0:
        results.append([])
    else:
        for x in range(min_div, n+1):
            temp = num_part_nodup(n-x, x)
            for d in temp:
                results.append([x]+d)
    return results

#number partition count
def num_part_nodup_cnt(n, min_div):
    cnt = 0
    if n==0:
        return 1
    else:
        for x in range(min_div, n+1):
            cnt += num_part_nodup_cnt(n-x, x)
    return cnt

# generate next partition
def num_part_generator(N):
    cur_part = [1]*N
    while len(cur_part) >= 1:
        yield cur_part
        if len(cur_part) == 1:
            break
        if cur_part[-1] - cur_part[-2] < 2:
            x = cur_part.pop(-1)
            cur_part[-1] += x
        else:
            cur_part[-2] += 1
            remain = cur_part.pop(-1)-1
            cur_min = cur_part[-1]
            while remain >= cur_min*2:
                cur_part.append(cur_min)
                remain -= cur_min
            if remain:
                cur_part.append(remain)
        
# other kinds of partitions can also be implemented according to relavant C++ algorithms

def test():
    print num_part_dup(5), num_part_dup_cnt(5)
    print num_part_nodup(5, 1), num_part_nodup_cnt(5, 1)
    print num_part_nodup(5, 2), num_part_nodup_cnt(5, 2)
    
    parts = num_part_generator(6)
    for p in parts:
        print p
    
if __name__ == '__main__':
    test()
    



