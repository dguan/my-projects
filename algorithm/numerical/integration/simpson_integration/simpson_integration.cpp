#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

static const int MAX_DIVIDE = 16;

////////////////////////////////////////////////////////////////////////
//
//    Numerical integration using Composite Simpson Rule
//
////////////////////////////////////////////////////////////////////////

double comp_simpson_integration(std::function<double(double)> func, double x_left, double x_right, int init_divide = 2, double precision = 1e-6)
{
	int div_cnt = init_divide;
	int cur_div = 1 << init_divide;

	double dx = (x_right - x_left) / cur_div;
	double step = dx + dx;

	double val_mul_4 = 0.0;
	double val_mul_2 = 0.0;
	double start_x_mul_4 = x_left + dx;
	double start_x_mul_2 = x_left + step;

	for (int i = 0; i < cur_div / 2 - 1; ++i)
	{
		val_mul_4 += func(start_x_mul_4);
		start_x_mul_4 += step;
		val_mul_2 += func(start_x_mul_2);
		start_x_mul_2 += step;
	}
	val_mul_4 += func(start_x_mul_4);
	double result = (func(x_left) + val_mul_4 * 4 + val_mul_2 * 2 + func(x_right)) * dx / 3.0;
	std::cout << "initial result is: " << result << std::endl;

	while (div_cnt < MAX_DIVIDE)
	{
		div_cnt++;
		cur_div *= 2;
		val_mul_4 = 0.0;
		val_mul_2 = 0.0;
		step = dx;
		dx *= 0.5;
		start_x_mul_4 = x_left + dx;
		start_x_mul_2 = x_left + step;

		for (int i = 0; i < cur_div / 2 - 1; ++i)
		{
			val_mul_4 += func(start_x_mul_4);
			start_x_mul_4 += step;
			val_mul_2 += func(start_x_mul_2);
			start_x_mul_2 += step;
		}
		val_mul_4 += func(start_x_mul_4);
		double temp_result = (val_mul_4 * 4 + val_mul_2 * 2 + func(x_left) + func(x_right))*dx / 3.0;
		double error = abs((temp_result - result) / (1 + abs(temp_result)));
		result = temp_result;
		if (error < precision)
			break;
	}
	std::cout << "div_cnt is: " << div_cnt << std::endl;
	if (div_cnt > MAX_DIVIDE)
		throw result;
	else
		return result;
}


int main()
{
	std::cout.precision(15);
	//std::cout << 0.0 / 0.0 << std::endl << std::endl;	// This line does not compile in VS2013
	std::cout << -std::sin(0.0) / 0.0 << std::endl << std::endl;
	std::cout << comp_simpson_integration([](double x) { return std::pow(x, 8); }, 0, 2, 4) << std::endl << std::endl;
	std::cout << comp_simpson_integration([](double x) { return std::sin(x); }, 0, 3.1416/4, 2, 1e-8) << std::endl << std::endl;
	
	// This example will show you the weakness of Simpson integration, when compiled with g++ 8.4, the answer is NAN, but the actual answer should be 3.70387410428452... something.
	// To be noted that VS2013 gives correct answer for this integration. This could be caused by the different way VS2013 and g++8.4 dealing with NAN.
	//
	// If you calculate this integration with order-4 Gauss-Legendre integration, there'll be no this problem at all! And that's the reason why I would prefer Gauss integration over simpson integration.
	std::cout << comp_simpson_integration([](double x) { return std::sin(x)/x; }, -3.1416, 3.1416) << std::endl << std::endl;
	
	// Or you can use modified integration function. In this situation, it's clear that Gauss-Legendre integration is superior in achieving desired precision.
	// But, this low precision may also be caused by my using relative-error instead of absolute-error, BUT, in Gauss-Legendre integration, I also used relative-error ......
	std::cout << comp_simpson_integration([](double x) { return std::sin(x)/x; }, -3.1416, 3.1416) << std::endl;									// This is 3.70442288646658
	std::cout << comp_simpson_integration([](double x) { return std::sin(x)/x; }, -3.1416, 3.1416, 4) << std::endl;									// This is 3.70387616999403
	std::cout << comp_simpson_integration([](double x) { if(x==0.0) return 1.0; return std::sin(x)/x; }, -3.1416, 3.1416, 4, 1e-9) << std::endl;	// This is 3.70387616999403

	return 0;
}


			
