#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
(c) david guan, gdyxgzy@hotmail.com
The complexity of the code is roughly O(10^n), where n is the total number of pattern chars and
wild cards in the 2 oprands. The pattern is also a big factor that could influence the complexity, for example,
'?? * ????? = #####' and 'abcde + cdefg = efgab' both have 7 patterns in the 2 operands, but the latter is 6 times faster than the former. Also, I divided the total solution space into 10 separate subspaces and, by using python multiprocessing, the time complexity can be reduced to about 1/NC, where NC is the number of cores/processors in the computer. Because my PC has only 4 cores, so 10 is enough, for larger scale problem, it can be divided into many small tasks and get processed using distributed processing.

TODO:
This is just an interesting question, anyway, I think I could improve this code by adding some features especially accepting expressions as oprands and result, or adding rational number support, this will enable this script to do some more complicated and powerful works such as finding Pythagorean Triples, solving equations, etc, or I can use this code as a basis to develop a small math game for children.

Test Cases:
1)	~ * ~ = ?? this will print none zero one digit multiplication table, from 1*1, 1*2, . . to 9*8, 9*9
2)	a * a = ?? : this will print the one digit square table, 0*0, 1*1, to 8*8, 9*9
3)	zZ * zZ = ???? : this will print square table from 00*00, 01*01, to 98*98, 99*99
4)	abcde * f = edcba : there is only one solution, 21978 * 4 = 87912
5)	??? * ????? = ##### : find out factors for numbers like 11111, 22222, to 99999. This is obviously an O(10^8) complexity problem, on my 3 GHz, 4 GB, Quad-Core PC it takes about 80 seconds.
6)	abcdef + cdefgh = efghij : This one is also an O(10^8) complexity problem, but because of the pattern constrain, it only takes about 9 seconds to find 2 solutions
7)	??? * ?????? = %%%%%% : this is an O(10^9) complexity problem, be warned that it takes about 900 seconds to run on my machine.
8)	Abcde + Fghij = $$$$$ : Although this one is seemingly an O(10^10) complexity problem, yet it only takes about 30 seconds to print 3072 solutions.
. . . . . .
"""

import re
import time
from sys import stderr as stderr # I just want to use an easy unbuffered output
import multiprocessing as multi_proc

pat_chars_normal = 'abcdefghijklmnopqrstuvwxyz'  # pattern characters
pat_chars_nz = 'ABCDEFGHI' # none zero pattern chars

wild_card = '?'          # wild card, can be anything from 0 -9
wild_card_nz = '~'       # none zero wild card, anything from 1 - 9
wc_uniq = 'QRSTUVWXYZ' # unique wild cards, if X is assigned 0, then Y cannot be 0
wc_uniq_nz = '@#$%^&*()' # none zero unique wild cards
wc_uniq_nz_re_str = '@#$%^&*\(\)'  # regex string for input checking

digits = '0123456789'
digits_nz = '123456789'

digits_set = set(digits)   # fast look-up table to check if a char is a digit
pat_char_set = set(pat_chars_normal+pat_chars_nz)   # fast look-up table for pattern chars
wc_uniq_char_set = set(wc_uniq + wc_uniq_nz)   # fast look-up table for uniq wild cards
nz_char_set = set(pat_chars_nz + wc_uniq_nz)   # fast look-up table for none-zero chars

def Usage() :
    print """ Welcome to secretSum, this program can find out solutions for your mysterious expressions.
    The expressions are like \'oprd1 op oprd2 = result\', like abcd + bcde = cdea, or zZ * zZ = ????
    Allowed operations are \'+\', \'-\', \'*\', \'/\', and \'%\', all operations are in integers.
    The meaning of allowed chars are explained as following :
      0 - 9 : digits, can be used anywhere, like a3 + b4 = 5c 
      a - z : normal unique pattern chars, can be 0--9
      A - I : 9 none zero unique pattern chars, can be 1--9
      ?, ~ : none pattern wild cards, ? can take any value, ~ can take any none zero value
      Q - Z : 10 none pattern unique wild cards, can substitute any pattern char, value range 0 -- 9
      @#$%^&*() : 9 none zero unique wild cards, can substitute any pattern char, value range 1 -- 9
    """ 

# if expected is not None, only verify if a match exists or not, otherwise return all availables
def find_availables(x_map, x_char, expected=None) :
    rslts = ''
    if not expected : # need to return all availables
        if not x_map.get(x_char):
            rslts = filter(lambda d : d not in x_map.values(), digits_nz if x_char in nz_char_set else digits)
        else : # This pattern has been already assigned a value
            rslts = x_map[x_char]
    else : #check if if there is a match for expected
        if not x_map.get(x_char) :
            if expected in x_map.values() :
                return None
            else :
                return rslts
        else:
            if x_map[x_char] !=expected :
                return None
    return rslts

# Recursively check if there exists a match for the target result    
def check_validity(pat_map, wc_map, cur_pat, target) :
    pat_idx = 0
    while pat_idx < len(cur_pat) :
        if cur_pat[pat_idx] in [wild_card, wild_card_nz] :
            if target[pat_idx]=='0' and cur_pat[pat_idx]==wild_card_nz :
                return False
            else:
                pat_idx += 1
                continue
        elif cur_pat[pat_idx] in digits_set :
            if cur_pat[pat_idx] != target[pat_idx] :
                return False
            else:
                pat_idx += 1
                continue
        elif cur_pat[pat_idx] in nz_char_set  and target[pat_idx]=='0' :
            return False
        else:
            break
    if len(cur_pat) == pat_idx :
        return True
    else:
        x_map = pat_map if cur_pat[pat_idx] in pat_char_set else wc_map
        if find_availables(x_map, cur_pat[pat_idx], target[pat_idx]) is None:
            return False
        else:
            x_map[cur_pat[pat_idx]] = target[pat_idx]
            return check_validity(pat_map, wc_map, cur_pat[pat_idx+1:], target[pat_idx+1:])

# The recursive function to generate numbers
def generate_nums(num_pat, pat_map, wc_map):
    results = []
    leading_str = ''
    availables = None
    
    pat_idx = 0
    while pat_idx < len(num_pat) :
        if num_pat[pat_idx] in digits_set :
            leading_str += num_pat[pat_idx]
            pat_idx += 1
        elif num_pat[pat_idx] in [wild_card, wild_card_nz] :
            availables=digits if num_pat[pat_idx]==wild_card else digits_nz
            pat_idx += 1
            break
        else :
            x_map = pat_map if num_pat[pat_idx] in pat_char_set else wc_map
            availables = find_availables(x_map, num_pat[pat_idx])
            pat_idx += 1
            break
            
    if len(num_pat)-pat_idx == 0 : #The num_pat has no pattern char, only digits
        if availables is None :
            results.append(leading_str)
        else :
            for ch in availables :
                results.append(leading_str+ch)
    elif num_pat[pat_idx-1] in [wild_card, wild_card_nz] : # wild card chars
        sub_pat_strs = generate_nums(num_pat[pat_idx:], pat_map, wc_map)
        if len(sub_pat_strs) > 0 :
            for ch in availables :
                for sps in sub_pat_strs :
                    results.append(leading_str+ch+sps)
    else :
        is_pat_char = num_pat[pat_idx-1] in pat_char_set
        x_map = pat_map.copy() if is_pat_char else wc_map.copy()
        for ch in availables :
            x_map[num_pat[pat_idx-1]] = ch
            sub_pat_strs = generate_nums(num_pat[pat_idx:], x_map, wc_map) if is_pat_char \
                else generate_nums(num_pat[pat_idx:], pat_map, x_map)
            for sps in sub_pat_strs :
                results.append(leading_str + ch + sps)
    return results

# the multiprocessing target function to get all results for given data range 
def get_results(result_queue, data_range, pats) :
    pat_map = {}
    wc_map = {}
    calculator = { '+': lambda a,b: str(a+b), '-': lambda a,b: str(a-b), \
                 '*': lambda a,b: str(a*b), '/': lambda a,b: str(a/b), \
                 '%': lambda a,b: str(a%b) }
    for n1 in data_range :
        for i, ch in enumerate(pats[0]) :
            if ch in pat_char_set :
                pat_map[ch] = n1[i]
            elif ch in wc_uniq_char_set :
                wc_map[ch] = n1[i]
            else :
                continue
                
        num2_strs = generate_nums(pats[2], pat_map, wc_map) # Find all available num2s according to current n1 pattern
        for n2 in num2_strs :
            try :
                rslt_str = calculator[pats[1]](int(n1), int(n2)) # Calculate the target result
            except :
                continue # Ignore illegal operations like div/mod by zero, and continue;
                
            if len(rslt_str)>len(pats[4]) : # This is obviously not a match, skip it
                continue
            elif len(rslt_str) < len(pats[4]) :
                rslt_str = '0'*(len(pats[4]) - len(rslt_str)) + rslt_str
            else :
                pass
                
            n2_pat_map = pat_map.copy()
            n2_wc_map = wc_map.copy()
            for i, ch in enumerate(pats[2]) :
                if ch in pat_char_set :
                    n2_pat_map[ch] = n2[i]
                elif ch in wc_uniq_char_set :
                    n2_wc_map[ch] = n2[i]
                else :
                    continue;
            if check_validity(n2_pat_map, n2_wc_map, pats[4], rslt_str) : # Bingo! this is a match
                result_queue.put(n1 + pats[1] + n2 + ' = ' + rslt_str)
    result_queue.put('DONE')


if __name__ == '__main__' :
    Usage()
    num_re_str = '[\w'+wild_card + wild_card_nz + wc_uniq_nz_re_str + ']+'
    re_str = '^\s*'+num_re_str+'\s+[+*/%-]\s+'+num_re_str+'\s+=\s+'+num_re_str+'\s*$' # regex for expression checking
    while True:
        expression = raw_input('Please input an expression(CR to quit): ')
        if expression=='' :
            break
        if re.match(re.compile(re_str), expression) is None:
            print 'There are errors in the expression, please retry'
            continue
        if len(set(expression) & pat_char_set) > 10 : # Have more than 10 pattern chars
            print 'There are more than 10 patterns chars, please retry'
            continue

        pats = re.split(r'\s*', re.sub(r'^\s+|\s*$', '', expression)) # split expression into 5 parts
        n1s = generate_nums(pats[0], {}, {}) # Find all available num1s using its pattern
        
        max_threads = 32
        n1_step = 1 if len(n1s)<=max_threads else int((len(n1s)+max_threads-1)/max_threads) # setup up to max_threads processes
        n1_div = range(0, len(n1s), n1_step) + [len(n1s)]
        processes = []
        active_proc_cnt = 0
        results = multi_proc.Queue()
        rslt_cnt = 0
        t0 = time.time()
        for i in range(len(n1_div)-1) :
            proc = multi_proc.Process(target=get_results, args=(results, n1s[n1_div[i] : n1_div[i+1]], pats))
            processes.append(proc)
            active_proc_cnt += 1
            proc.start()
        while active_proc_cnt > 0 :
            while not results.empty() :
                s = results.get()
                if s == 'DONE' :
                    active_proc_cnt -= 1
                else :
                    stderr.write('\n'+s)
                    rslt_cnt += 1
            if not active_proc_cnt :
                print
                break;
            else :
                stderr.write('.')
                time.sleep(0.1)
        for p in processes :
            p.join()
            
        if not rslt_cnt :
            print 'No solution for %s in %f seconds' % (expression, time.time()-t0)
        else :
            print '%d solutions for %s in %f seconds' % (rslt_cnt, expression, time.time()-t0)
    
    print 'Thanks for using, bye!'

