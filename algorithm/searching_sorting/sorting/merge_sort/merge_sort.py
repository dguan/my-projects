import random

def merge_sort(data, length):
    if length<=1:
        return

    temp = [None]*length
    src = data
    dst = temp
        
    def merge(low1, high1, low2, high2):
        start_pos=low1
        pos = low1
        while low1<=high1 and low2<=high2:
            if src[low1]<=src[low2]:
                dst[pos] = src[low1]
                pos += 1
                low1 += 1
            else:
                dst[pos] = src[low2]
                pos += 1
                low2 += 1
                
        if low1>high1:
            while low2<=high2:
                dst[pos] = src[low2]
                pos += 1
                low2 += 1
        else:
            while low1<=high1:
                dst[pos] = src[low1]
                pos += 1
                low1 += 1
    # -- end of function merge --

    temp = [None]*length
    src = data
    dst = temp
    
    step = 1
    round_cnt = 0
    while True:
        low1 = 0
        while low1<=length-1:
            high1 = low1+step-1
            if high1 >= length-1:
                high1 = length-1
            low2 = high1+1
            if low2 >= length:
                for i in range(low1, high1+1):
                    dst[i] = src[i]
                break
            else:
                high2 = low2+step-1
                if high2 >=length-1:
                    high2 = length-1
                    
            merge(low1, high1, low2, high2)
            low1 = high2+1
            
        step *= 2
        if step>(length+1)/2 and high2>=length-1:
            if not round_cnt%2:
                for i in range(length):
                    src[i] = dst[i]
            return
        else:
            round_cnt += 1
            src, dst = dst, src

def main() :
    data = []

    for i in range(17):
        data.append(random.randrange(0,97))
    print data
    print
    merge_sort(data, len(data))
    print data
    print

if __name__ == "__main__" :
    main()

