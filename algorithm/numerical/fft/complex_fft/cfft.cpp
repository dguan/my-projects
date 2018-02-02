#include "complex.h"
#include <type_traits>

static const int N=32;

static Complex fft_coeff[N];
static Complex ifft_coeff[N];

void setup_table(void)
{
	for(int i=0; i<N; ++i)
	{
		fft_coeff[i] = Complex(1.0, -PI*i*2/N, true);
		ifft_coeff[i] = Complex(1.0, PI*i*2/N, true);
	}
}

void decimation(Complex data[])
{
	int r=0;
	int i=1;
//#define swp(x, y)	{Complex temp=(x); (x)=(y); (y)=temp;}

	while(i<N/2)
	{
		r += N/2;
		std::swap(data[i], data[r]);
		++i;

		for(int m=N>>1; !((r^=m)&m); m>>=1);
		if(r>i)
		{
			std::swap(data[i], data[r]);

			std::swap(data[N-1-i], data[N-1-r]);
		}
		++i;
	}
//#undef swp
}

void fft_dit(Complex data[], bool ifft=false)
{
	decimation(data);

	Complex *coeff;
	if(ifft)
		coeff = ifft_coeff;
	else
		coeff = fft_coeff;

	int groups=N/2;
	int step = 2;
	while(groups>=1)
	{
		for(int i=0; i<groups; ++i)
		{
			int first = i*step;
			int second = first+step/2;
			int coeff_idx = 0;
			for(int j=0; j<step/2; ++j)
			{
				Complex temp = data[second]*coeff[coeff_idx];

				data[second] = data[first] - temp;
				data[first]  = data[first] + temp;

				if(ifft==true)
				{
					data[first] = data[first]/2;
					data[second] = data[second]/2;
				}

				++first;
				++second;
				coeff_idx += groups;
			}
		}
		groups /= 2;
		step *= 2;
	}
}

void fft_dif(Complex data[], bool ifft=false)
{
	Complex *coeff;
	if(ifft)
		coeff = ifft_coeff;
	else
		coeff = fft_coeff;

	int groups = 1;
	int step = N;

	while(groups<=N/2)
	{
		for(int i=0; i<groups; ++i)
		{
			int first = i*step;
			int second = first+step/2;
			int coeff_idx = 0;

			for(int j=0; j<step/2; ++j)
			{
				Complex temp = data[first];
				data[first] = data[first]+data[second];
				data[second] = (temp-data[second])*coeff[coeff_idx];

				if(ifft)
				{
					data[first] = data[first]/2;
					data[second] = data[second]/2;
				}

				++first;
				++second;
				coeff_idx += groups;
			}
		}
		groups *= 2;
		step /= 2;
	}

	decimation(data);
}

int main()
{
    //test Complex class traits

	std::cout << "Is Complex class Trivial? " << (std::is_trivial<Complex>::value ? "YES" : "NO") << std::endl;
	// g++ 8.4.2 still does not  support is_trivially_copyable yet, so this line will not compile in g++, but it compiles in VS 2013, and the answer is 'YES'
	//std::cout << "Is Complex class TriviallyCopyable? " << (std::is_trivially_copyable<Complex>::value ? "YES" : "NO") << std::endl;

	std::cout << "Is Complex class MoveAssignable? " << (std::is_move_assignable<Complex>::value ? "YES" : "NO") << std::endl;
	std::cout << "Is Complex class MoveConstructible? " << (std::is_move_constructible<Complex>::value ? "YES" : "NO") << std::endl;

	std::cout << "Is Complex class Standard Layout? " << (std::is_standard_layout<Complex>::value ? "YES" : "NO") << std::endl;
	std::cout << "Is Complex class POD? " << (std::is_pod<Complex>::value ? "YES" : "NO") << std::endl;

/*
	// Complex class is not trivial, but is trivially copyable. is move_assignable, and is move_constructible.
	// is standard_layout, but not POD. So, the good news: std::swap can be applied to Complex class,
	// and also Complex class is std::memmove-able and std::memcpy-able, this is nice.

	Complex a(3,4);
	Complex b = a;
	Complex c = a;
	c.to_polar();
	b = conj(a);
	c.conj();

	if(b==c)
		cout<<"b==c"<<endl;
	else
		cout<<"b!=c, @#$%!??????......"<<endl;
*/
	setup_table();

	Complex raw_data[N];
	for(int i=0; i<N; ++i)
		raw_data[i] = i+1;

	// Display raw data
	std::cout << "The data to be fft-ed are:" << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i]<<' ';
	std::cout << std::endl;

	// FFT raw data and display
	fft_dit(raw_data);
	std::cout << "The fft result are:" << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ' ';
	std::cout << std::endl;

	// IFFT back to raw data and display
	fft_dit(raw_data, true);
	std::cout << "The ifft-ed data are:" << std::endl;
	for(int i=0; i<N; ++i)
		std::cout << raw_data[i] << ' ';
	std::cout << std::endl;

	return 0;
}

