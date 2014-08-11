The implementation of fixed_point FFT algorithm and real-time convolution algorithm. There's an improvement in the digital convolution over the ordinary 'overlap-save' or 'overlap-add' algorithm.

This implementation utilised fixed-point algorithms and is VERY efficient. The only draw-back is that it need to be careful of multiplication overflow when dealing with long sequence or high precisions. I am using 15-digit pseudo floating-point coefficients, the precision is upto 0.00003. Under this precision, the longest FFT sequence is 32 for 12-bit ADC inputs.

Yet, it should have no problem with the real-time convolution algorithm, because it's seldom to have a long filter, and the length of the data frame can be adjusted without (much) impact with the algorithm's performance.

