# generate binomial coefficients of order N
def binomial_seq(N):
    coeff = [1]*(N+1)
    for i in xrange(1, N+1):
        temp = coeff[0]
        for j in xrange(1, i):
            temp, coeff[j] =  coeff[j], coeff[j] + temp
    return coeff

# generate binomial sequences
def binomial_seq_generator():
    N = 0
    coeff = [1]
    while True:
        yield coeff
        N += 1
        coeff.append(1)
        temp = coeff[0]
        for i in xrange(1, N):
            temp, coeff[i] =  coeff[i], coeff[i] + temp

if __name__ == '__main__':
    for i in xrange(10):
        print binomial_seq(i)

    x = binomial_seq_generator()
    for i in xrange(10):
        print x.next()



