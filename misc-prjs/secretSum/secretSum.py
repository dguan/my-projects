#!/usr/bin/python

"""
(c) david guan, dgacfr@gmail.com, gdyxgzy@hotmail.com

This code has been tested with Python 2.7.6, the complexity of the code is roughly O(10^n), where n is the
total number of unique pattern chars and wild cards in the 2 oprands. Also, the complexity can be largely
influenced by the pattern, for example, '?? * ????? = #####' and 'abcde + cdefg = efgab' both have 7 patterns
in the 2 operands, but the latter takes only about 1/4 time of the former. Further, multiprocessing can
reduce the time complexity to about 1/NC, where NC is the number of cores/processors in the computer.

TODO:
Well, this code is just for fun, yet it still could be extended to some extent, the most important feature
should be accepting expressions as oprands and result, so it can do some more powerful work such as finding
Pythagorean Triples or solve equations, etc. And then I think I could use this code as basis to develop a
small math game for children. As for engineers, they have Matlab already ......
"""

import re
import time
from sys import stderr as stderr # I just want to use an easy unbuffered output
import multiprocessing as multi_proc

### In order not to make this script too long, some docs and comments are not included ...

pat_chars_normal = 'abcdefghijklmnopqrstuvwxyz'
pat_chars_nz = 'ABCDEFGHI'

#wild card chars
wild_card = '?'
wild_card_nz = '~'
wc_uniq = 'QRSTUVWXYZ'
wc_uniq_nz = '@#$%^&*()'
wc_uniq_nz_re_str = '@#$%^&*\(\)' # for regex checking

digits = '0123456789'
digits_nz = '123456789'

digits_set = set(digits) # fast look-up table to check if a char is a digit
pat_char_set = set(pat_chars_normal+pat_chars_nz) # fast look-up table for pattern chars
wc_uniq_char_set = set(wc_uniq + wc_uniq_nz) #fast look-up table for uniq wild cards
nz_char_set = set(pat_chars_nz + wc_uniq_nz) # fast look-up table for none-zero chars

def Usage() :
    print 'Welcome to secretSum, this program can find out solutions for your mysterious expressions.'
    print 'The expressions are like \'oprand1 op oprand2 = result\', like abcd + bcde = cdea, or zZ * zZ = ????'
    print 'Allowed operations are \'+\', \'-\', \'*\', \'/\', and \'%\', all operations are in integers.'
    print 'The meaning of allowed chars are explained as following :'
    print '      0 - 9 : digits, can be used any where, like a3 + b4 = 5c '
    print '      a - z : normal unique pattern chars, can be 0--9 '
    print '      A - I : 9 none zero unique pattern chars, can be 1--9 '
    print '       ?, ~ : none pattern wild cards, ? can take any value, ~ can take any none zero value'
    print '      Q - Z : 10 none pattern unique wild cards, can substitute any pattern char, value value 0--9'
    print '  @#$%^&*() : 9 none zero unique wild cards, can substitute any pattern char, value range 1 -- 9'

# if expected is not None, only verify if a match exists or not, otherwise return all availables
def find_availables(x_map, x_char, expected=None) :
    rslts = ''
    if expected is None : # need to return all availables
        if x_map.get(x_char) is None:
            rslts = filter(lambda d : d not in x_map.values(), digits_nz if x_char in nz_char_set else digits)
        else : # This pattern has been already assigned a value
            rslts = x_map[x_char]
    else : #check if if there is a match for expected
        if x_map.get(x_char) is None :
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
    if len(cur_pat)-pat_idx == 0 :
        return True
    else:
        x_map = pat_map if cur_pat[pat_idx] in pat_char_set else wc_map
        if find_availables(x_map, cur_pat[pat_idx], target[pat_idx]) is None :
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

        n1_step = 1 if len(n1s)<=10 else int((len(n1s)+9)/10) # setup up to 10 processes
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
            if active_proc_cnt == 0:
                print
                break;
            else :
                stderr.write('.')
                time.sleep(0.1)
        for p in processes :
            p.join()
            
        if rslt_cnt == 0 :
            print 'No solution for %s in %f seconds' % (expression, time.time()-t0)
        else :
            print '%d solutions for %s in %f seconds' % (rslt_cnt, expression, time.time()-t0)
    
    print 'Thanks for using, bye!'

