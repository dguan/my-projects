#include <iostream>
#include <cmath>
//#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////
//
// This is transformed from the code I wrote for an ARM processor, so it has something with the 
// target processor's architecture, processor's word length, ADC's digits, etc. And above all,
// the processor should have a 32-bit hardware multiplier to achieve high performance.
//
// Here I improved this fixed-point fft algorithm with new C++11 int64_t data type, extended
// the word length to int64_t, so it is able to deal with larger data length and higher precision.
// 
//////////////////////////////////////////////////////////////////////////////////////////////


//
// do fixed point FFT using Decimation-In-Frequency
//
// re-written in C++, yet it should be easy to change it into C. 
//

static const double PI = 3.1415926576;

template<int N> struct N_BITS {	enum {val = 1 + N_BITS<N/2>::val}; };
template<> struct N_BITS<0> { enum {val = 0}; };

static const int N=32;	//32 point FFT/IFFT
static const int BITS=N_BITS<N-1>::val;	//number of bits of N

static const int PRECISION=15;	// 15 precision bits plus 1 sign bit, 16 bits to fit in a short int
//15 floating-point digits, the precision is 1/32768,
//under most circumstances, this precision is more than
//enough for industrial control. After all, we often use
//10-bit or 12-bit ADC, sometimes only 8-bit ADC, whose precision
//is only 1/1024, 1/4096, or even ony 1/256, the overall precision
//is determined by the ADC.
//
//And, because we're using fixed-point
//algorithm, final results will be changed back into unsigned data,
//the first floating points are rounded in, others are discarded,
//So in practice we set PRECISION as needed like 11, 12, only not to lose
//precision too much, that's enough.
//
//if PRECISION is set to 12, then we can do 256-point FFT/IFFT
//for 12-bit ADC samples without worrying for multiplication overflow.

typedef short COEFF;
typedef int DATA;

//FFT coefficients
static COEFF coeff_r[N/2], coeff_i[N/2];

//input sample data, real part and imaginary part, has been shifted PRECISION digits
static DATA _data_r[N], _data_i[N];

//setup coefficients, and change it into right format, so we can use fixed-point multiplication
void setup_table(COEFF cff_r[], COEFF cff_i[], int n)
{
	double omega = -PI*2/N;
	double theta = 0;
	for(int i=0; i<(n>>1); ++i)
	{
		cff_r[i] = (COEFF)(cos(theta)*((1<<PRECISION)-1));
		cff_i[i] = (COEFF)(sin(theta)*((1<<PRECISION)-1));

		theta += omega;
	}
}

//do decimation
#define swp(x, y) { DATA temp; temp=*(x); *(x)=*(y); *(y)=temp; }
void decimation(DATA data[], int n)
{
	int ir=0;
	int t=n/2;
	int i=1;
	while(i<t)
	{
		//i odd
		ir += t;
		swp(&data[i], &data[ir]);
		++i;

		//i even
		for(int x=t; !((ir^=x) & x); x>>=1);

		if(ir > i)
		{
			swp(&data[i], &data[ir]);
			swp(&data[n-1-i], &data[n-1-ir]);
		}
		++i;
	}
}

//
//FFT -- decimation in frequency
//by using fixed-point algorithm and high-performance decimation,
//it's quite efficient. Of course, in practice, we use some other
//tricks to improve its performance.
//
//btw, mordern compilers can use shift to calculate *2 or /2,
//so I can happily write 'i*=2' instead of 'i<<=1'.
//
void fft_dif(DATA data_r[], DATA data_i[], int n)
{
	int groups = 1;
	int step = n;

	while(groups<n)
	{
		for(int i=0; i<groups; ++i)
		{
			int first = i*step;
			int second = first+step/2;
			int coeff_idx = 0;
			for(int j=0; j<step/2; ++j)
			{
				DATA temp_r = (DATA)(((long long)(data_r[first+j]-data_r[second+j])*coeff_r[coeff_idx] - (long long)(data_i[first+j]-data_i[second+j])*coeff_i[coeff_idx])>>PRECISION);
				DATA temp_i = (DATA)(((long long)(data_r[first+j]-data_r[second+j])*coeff_i[coeff_idx] + (long long)(data_i[first+j]-data_i[second+j])*coeff_r[coeff_idx])>>PRECISION);

				data_r[first+j] += data_r[second+j];
				data_i[first+j] += data_i[second+j];

				data_r[second+j] = temp_r;
				data_i[second+j] = temp_i;

				coeff_idx += groups;
			}
			first += step;
			second += step;
		}
		groups *= 2;
		step /= 2;
	}

	decimation(data_r, n);
	decimation(data_i, n);
}

//
// suppose that input data are from ADC, 12bits, left shifted PRECISION bits and saved as integer;
// system function h(n) are integers, with PRECISION bits of floating-point digits. As we defined PRECISION
// as 15 bits, so, must be careful of overflow. 32-12-15-1 = 4, so, if the length of h(n) >16, there
// may be the possibility of overflow. To increase the length, try to decrease the float point digits,
// or change the result back to integer after each mul-add, or, keep only a few float point digits(say, 5 bits)
// during mul-add to increase precision, then, after all mul-add are done, change the result into integer.
// And, the best way, calculate the intermediate result in long long, then change the result into integer.
//
// the buffer's structure is as below:
// |----------------------------------------------------------------|
// |  (hn_n - 1) remaining data  |      (data_n) incoming data      |
// |----------------------------------------------------------------|
//                               | <-- the start of data[]
//
// after calculated data_n points, copy the last (hn_n-1) to front
// as remaining data to be calculated in next frame of data.
//
// I think this is, and by test it is, an improvement to normal overlap-save convolution,
// it can calculate more data in one convolution round, and, compared to overlap-add algorithm,
// copying data to the remaining area should be cheaper compared to add convolution result
// with remaining partial data.
//
void conv(int output[], int data[], int hn[], int data_n, int hn_n)
{
	for(int i=0; i<data_n; ++i)		// calculate data_n points
	{
		long long temp = 0;

		// calculate convolution
		for(int j=0; j<hn_n; ++j)
			temp += hn[hn_n-1-j]*data[i+j-hn_n+1];

		//copy into output
		output[i] = (int)(temp>>PRECISION);
	}

	// copy the remaining data
	memcpy(data-(hn_n-1), data+(data_n-hn_n+1), (hn_n-1)*sizeof(int));
}

int main()
{
	setup_table(coeff_r, coeff_i, N);

	for(int i=0; i<N; ++i)
	{
	//
	//the following data settings are for N=32 fft and ifft,
	//supposing that we are using data from 12-bit ADC, left
	//shifted PRECISION digits, as if that it has PRECISION digits
	//of floating point digits, so we can use fixed-point multiplication
	//to calculate FFT and IFFT.
	//
	//note that we subtracted 2048 on each data, to transform
	//it from 12-bit unsigned data into a 11-bit signed data,
	//otherwise, if we do not subtract 2048 from the data,
	//we can only do 16-point FFT and IFFT, 32-point FFT and IFFT
	//will overflow.
	//
		_data_r[i] = (i+1+(4095-N/2)-2048)<<PRECISION;
		_data_i[i] = (0-2048)<<PRECISION;
	}

	//do FFT for input data
	fft_dif(_data_r, _data_i, N);
	//do IFFT using the same FFT function and same FFT coefficients, only change the order of input
	fft_dif(_data_i, _data_r, N);


	//change the data back into 12-bit unsigned data, it's exactly the same
	//as input data after FFT and IFFT, that's another benefit of using
	//fixed-point algorithm over floating-point algorithm.
	DATA result_r[N], result_i[N];
	for(int i=0; i<N; ++i)
	{
		result_r[i] = (_data_r[i]+(1<<(PRECISION+BITS-1))) >> (PRECISION+BITS);
		result_i[i] = (_data_i[i]+(1<<(PRECISION+BITS-1))) >> (PRECISION+BITS);

		result_r[i] += 2048;
		result_i[i] += 2048;
	}

	cout<<"The result after FFT and IFFT is:"<<endl;
	for(int i=0; i<N; ++i)
		cout<<result_r[i]<<',';
	cout<<endl;

	//suppose impulse response is 3, data frame is 8
	int hn[] = {1*((1<<PRECISION)), -1*((1<<PRECISION)), 2*((1<<PRECISION))};
	int conv_data[8+2];
	int conv_output[8+8];
	int *p_data = &conv_data[sizeof(hn)/sizeof(int)-1];

	// the initial remaining data is 0
	conv_data[0] = 0;
	conv_data[1] = 0;

	for(int i=0; i<8; ++i)
		conv_data[i+2] = i+1;
	conv(conv_output, p_data, hn, 8, sizeof(hn)/sizeof(int));

	for(int i=0; i<8; ++i)
		conv_data[i+2] = 8-i;

	//do convolution
	conv(&conv_output[8], p_data, hn, 8, sizeof(hn)/sizeof(int));

	cout<<"The convolution result is:"<<endl;
	for(int i=0; i<8+8; ++i)
		cout<<conv_output[i]<<',';
	cout<<endl;

	return 0;
}

