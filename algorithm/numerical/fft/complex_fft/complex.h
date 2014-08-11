#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <cmath>
#include <cassert>
#include <iostream>

static const double PI = 3.1415926536;

class Complex
{
private:
	double real;
	double img;
	bool is_polar;
	double epsilon;
	
	#define IsZero(x) ((bool) ((x)<epsilon && (x)>(-epsilon)))
	void check_zero(void);

public:
	Complex() : real(0.0), img(0.0), is_polar(false), epsilon(1e-8) {};
	
	// If use this default c-tor, this class can be trivially constructible and trivially default constructible.
	// Anyway, we still have the trivially copyability. 
	//Complex() = default;

	//no explicit here, otherwise you cannot use codes like Complex X=32; etc.
	//Complex(int r) : real(r), img(0.0), is_polar(false) {};
	Complex(double r) : real(r), img(0.0), is_polar(false), epsilon(1e-8) { check_zero(); };

	Complex(double r, double i, bool polar=false, double precision=1e-8) : real(r), img(i), is_polar(polar), epsilon(precision)
	{ if(is_polar) assert(real>0.0); check_zero(); };

	//this constructor not used yet, just comment it out and keep it here
	//Complex(std::pair<double, double> polar_input) : real(polar_input.first), img(polar_input.second), is_polar(true) { check_zero(); };

	inline const double get_real() const;
	inline const double get_img() const;
	inline const double get_rho() const;
	inline const double get_theta() const;
	inline const bool IsPolar() const { return is_polar; };

	void to_polar(void);
	void to_ri(void);
	void conj(void) { img = -img; };

	const Complex& operator = (double a);
	const Complex& operator = (const Complex& a);
	const bool operator == (const Complex& a) const;

	const Complex operator -() const;

	const Complex operator +(const Complex& a) const;
	const Complex operator -(const Complex& a) const;
	//Complex operator *(double a) { return Complex(get_real()*a, get_img()*a); };
	const Complex operator *(double a) const;
	const Complex operator *(const Complex& a) const;
	//Complex operator /(double a) { return Complex(get_real()/a, get_img()/a); };
	const Complex operator /(double a) const;
	const Complex operator /(const Complex& a) const;

	friend std::ostream& operator <<(std::ostream& os, const Complex& a);

	friend const Complex conj(const Complex& a);
};

void Complex::check_zero(void)
{
	if(IsZero(real))
		real = 0.0;
	if(IsZero(img))
		img = 0.0;
}

void Complex::to_polar(void)
{
	if(is_polar==false)
	{
		double temp_real = get_rho();
		img = get_theta();
		real = temp_real;
		is_polar = true;
	}
}

void Complex::to_ri(void)
{
	if(is_polar)
	{
		double temp_real = get_real();
		img = get_img();
		real = temp_real;
		is_polar = false;
	}
}

const double Complex::get_real() const
{
	return (is_polar ? real*::cos(img):real);
}

const double Complex::get_img() const
{
	return (is_polar ? real*::sin(img):img);
}

const double Complex::get_rho() const
{
	return (is_polar ? real:std::sqrt(real*real+img*img));
}

const double Complex::get_theta() const
{
	if(is_polar)
		return img;
	else
		return ::atan2(img, real);
}

inline const Complex& Complex::operator = (double a)
{
	is_polar = false;
	real = a;
	img = 0;

	return *this;
}

inline const Complex& Complex::operator = (const Complex& a)
{
	is_polar = a.IsPolar();
	real = (is_polar ? a.get_rho():a.get_real());
	img = (is_polar ? a.get_theta():a.get_img());
	if(epsilon > a.epsilon)
		epsilon = a.epsilon;

	return *this;
}

inline const bool Complex::operator == (const Complex& a) const
{
	if(is_polar == a.is_polar)
		return (IsZero(real-a.real) && IsZero(img-a.img));
	else
		return (IsZero(get_real()-a.get_real()) && IsZero(get_img()-a.get_img()));
}

inline const Complex Complex::operator -() const
{
	return (is_polar ? Complex(real, PI+img, true):Complex(-real, -img, false));
}

const Complex Complex::operator +(const Complex& a) const
{
	Complex temp(get_real()+a.get_real(), get_img()+a.get_img(), false);
	if(is_polar)
		temp.to_polar();
	return temp;
}

const Complex Complex::operator -(const Complex& a) const
{
	Complex temp(get_real()-a.get_real(), get_img()-a.get_img(), false);
	if(is_polar)
		temp.to_polar();
	return temp;
}

const Complex Complex::operator *(double a) const
{
	assert(!IsZero(a));

	if(is_polar)
		return Complex(get_rho()*a, get_theta(), true);
	else
		return Complex(get_real()*a, get_img()*a, false);
}

const Complex Complex::operator *(const Complex& a) const
{
	if(is_polar)
		return Complex(get_rho()*a.get_rho(), get_theta()+a.get_theta(), true);
	else
		return Complex(get_real()*a.get_real()-get_img()*a.get_img(), get_real()*a.get_img()+get_img()*a.get_real(), false);
}

const Complex Complex::operator /(double a) const
{
	assert(!IsZero(a));

	if(is_polar)
		return Complex(get_rho()/a, get_theta(), true);
	else
		return Complex(get_real()/a, get_img()/a, false);
}

const Complex Complex::operator /(const Complex& a) const
{
	Complex temp(get_rho()/a.get_rho(), get_theta()-a.get_theta(), true);
	if(is_polar==false)
		temp.to_ri();
	return temp;
}

std::ostream& operator <<(std::ostream& os, const Complex& a)
{
	if(a.IsPolar()==false)
		os<<'('<<a.get_real()<<"r,"<<a.get_img()<<"i)";
	else
		os<<'<'<<a.get_rho()<<"h,"<<a.get_theta()<<"t)";
	return os;
}

inline const Complex conj(const Complex& a)
{
	return Complex(a.real, -a.img, a.is_polar);
}

#endif
