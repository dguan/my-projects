# generate the n-th Bell numbers
def bell_number(N):
    bell = [1]
    if not N:
        return bell[0]
    for i in xrange(1, N+1):
        x = bell[-1]
        bell.append(0)
        for j in xrange(0, i):
            x, bell[j] = bell[j]+x, x
        bell[i] = x
    return bell[N]

# generate the Bell number sequences
def bell_seq_generator():
    bell = [1]
    N = 0
    while True:
        yield bell[N]
        x = bell[-1]
        bell.append(0)
        N += 1
        for j in xrange(0, N):
            x, bell[j] = bell[j]+x, x
        bell[N] = x



if __name__ == '__main__':
    for i in range(10):
        print bell_number(i)
    x = bell_seq_generator()
    for i in xrange(10):
        print x.next()

