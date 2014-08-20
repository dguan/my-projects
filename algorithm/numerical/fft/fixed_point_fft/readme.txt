The implementation of fixed_point FFT algorithm and real-time convolution algorithm. There's an improvement in the digital convolution over the ordinary 'overlap-save' or 'overlap-add' algorithm.

This implementation utilised fixed-point algorithms and is VERY efficient. The only draw-back is that it need to be careful of multiplication overflow when dealing with long sequence or high precisions. I am using 15-digit pseudo floating-point coefficients, the precision is upto 0.0003, which should be quite enough for normal industrial usage, after all, ordinary ADC for industrial control is only 12-bits, corresponding to a precision of 1/4096, this is dicisive for overall precision. Under this 15-bit precision and 12-bit ADC input, the longest FFT sequence is 32.

Yet, it should have no problem with the real-time convolution algorithm, because it's seldom to have a long filter, and the length of the data frame can be adjusted without (much) impact with the algorithm's performance.

Also, this FFT sequence's length can be improved dramaatically by using 64-bit integer multiplicaiton or simulated 64-bit integer multiplication, it will be more convinient using certain inline assembly language programming and certain knowledge of processor architecture, and will be a little bit hard to write portable C/C++ code, so I will leave this as is for now, will do it some time later.

