# generate Catalan sequence from 0 to N
def catalan_seq(N):
    cat = [1]
    if not N:
        return cat
    for i in xrange(1, N+1):
        cat.append(0)
        for j in xrange(0, i):
            cat[i] += cat[j]*cat[i-1-j]
    return cat

# generate Catalan sequences
def catalan_seq_generator():
    N = 0
    cat = [1]
    while True:
        yield cat
        cat.append(0)
        N += 1
        for i in xrange(0, N):
           cat[N] += cat[i]*cat[N-1-i]



if __name__ == '__main__':
    for i in xrange(10):
        print catalan_seq(i)

    x = catalan_seq_generator()
    for i in xrange(10):
        print x.next()



