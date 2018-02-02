# -*- coding: utf-8 -*-
"""
Created on Fri May 02 15:45:37 2014

@author: dguan
"""
import random
import cProfile

def build_heap(heap, data):
    heap.append(data)
    data_pos = len(heap)-1
    parent = (data_pos-1)/2
    while parent>=0:
        biggest = parent
        left = parent*2+1
        right = left+1
        if left<=data_pos and heap[left]>heap[biggest]:
            biggest = left
        if right<=data_pos and heap[right]>heap[biggest]:
            biggest = right
        if biggest!=parent:
            heap[parent], heap[biggest] = heap[biggest], heap[parent]
            parent = (parent-1)/2
        else:
            break
    #print heap
    

def heap_sort(heap):
    cur_len = len(heap)
    while cur_len > 1:
        heap[cur_len-1], heap[0] = heap[0], heap[cur_len-1]
        cur_len -= 1
        cur_parent = 0
        while cur_parent<=(cur_len-1)/2:
            biggest = cur_parent
            left = cur_parent*2+1
            right = left+1
            if left<cur_len and heap[left]>heap[biggest]:
                biggest = left
            if right<cur_len and heap[right]>heap[biggest]:
                biggest = right
            if biggest != cur_parent:
                heap[cur_parent], heap[biggest] = heap[biggest], heap[cur_parent]
                cur_parent = biggest
            else:
                break
        #print heap

def main() :
    raw_data = []
    for i in range(17):
        build_heap(raw_data, random.randrange(997))
    print "Built up heap is: ", raw_data
    heap_sort(raw_data)
    print "Sorted data is: ", raw_data
    
def profile_heap_sort() :
    heap_sort_data = []
    for i in xrange(100000):
        build_heap(heap_sort_data, random.randrange(99991))
    heap_sort(heap_sort_data)

def profile_builtin_sort() :
    builtin_sort_data = []
    for i in range(100000):
        builtin_sort_data.append(random.randrange(99991))
    #builtin_sort_data.sort()
    sorted(builtin_sort_data)


if __name__ == "__main__" :
    main()
    print
    cProfile.run('profile_heap_sort()')
    print
    cProfile.run('profile_builtin_sort()')
    

