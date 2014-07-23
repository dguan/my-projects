#include "complex.h"

using namespace std;

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
	setup_table();

	Complex raw_data[N];
	for(int i=0; i<N; ++i)
		raw_data[i] = i+1;

	cout<<"The data to be fft-ed are:"<<endl;
	for(int i=0; i<N; ++i)
		cout<<raw_data[i]<<' ';
	cout<<endl;

	fft_dit(raw_data);
	cout<<"The fft result are:"<<endl;
	for(int i=0; i<N; ++i)
		cout<<raw_data[i]<<' ';
	cout<<endl;

	fft_dit(raw_data, true);
	//swp(raw_data[0], raw_data[1]);
	cout<<"The ifft-ed data are:"<<endl;
	for(int i=0; i<N; ++i)
		cout<<raw_data[i]<<' ';
	cout<<endl;

	Complex xx=-10;
	
	double length_xx = xx.get_rho();
	double angle_xx = xx.get_theta();

	Complex yy(0, -10);
	double length_yy = yy.get_rho();
	double angle_yy = yy.get_theta();

	//Complex zz(-3, PI, true);

	Complex p(3,3);
	Complex q(-3,4);
	Complex r(-4,-4);
	Complex s(4, -3);
	Complex t = conj(p+q)+2.5;

	r+s;
	-(p+q*r);


	p.to_polar();
	q.to_polar();
	r.to_polar();
	s.to_polar();

	p.to_ri();
	q.to_ri();
	r.to_ri();
	s.to_ri();



	t = s = r = p;
	t = s*2;
	t = s/10;

	t = s+3;
	t = -t;
	//-t = p;


	Complex a(3,4);
	Complex b = a;
	Complex c = a;
	c.to_polar();

	b = conj(a);
	c.conj();

	double dddd = a.get_real();

	if(b==c)
		cout<<"b==c"<<endl;
	else
		cout<<"b!=c, @#$%!?"<<endl;

	return 0;
}

