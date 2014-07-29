#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <functional>
#include <cmath>

// Normalized Legendre Nodes
std::vector<std::vector<double>> legendre_nodes = { { 0 }, \
	{ -0.5773502692, 0.5773502692 }, \
	{ -0.7745966692, 0, 0.7745966692 }, \
	{ -0.8611363116, -0.3399810436, 0.3399810436, 0.8611363116 }, \
	{ -0.9061798459, -0.5384693101, 0, 0.5384693101, 0.9061798459 }, \
	{-0.9324695142, -0.6612093865, -0.2386191861, 0.2386191861, 0.6612093865, 0.9324695142}, \
	{-0.9491079123, -0.7415311856, -0.4058451514, 0, 0.4058451514, 0.7415311856, 0.9491079123}, \
	{-0.9602898565, -0.7966664774, -0.5255324099, -0.1834346425, 0.1834346425, 0.5255324099, 0.7966664774, 0.9602898565}, \
	{-0.9681602395, -0.8360311073, -0.6133714327, -0.3242534234, 0, 0.3242534234, 0.6133714327, 0.8360311073, 0.9681602395}, \
	{-0.9739065285, -0.8650633667, -0.6794095683, -0.4338953941, -0.1488743390, 0.1488743390, 0.4338953941, 0.6794095683, 0.8650633667, 0.9739065285 } };

// Normalized Legendre Coefficients
std::vector<std::vector<double>> legendre_coeffs = { { 2 }, \
	{ 1, 1 }, \
	{ 5 / 9.0, 8 / 9.0, 5 / 9.0 }, \
	{ 0.3478548451, 0.6521451549, 0.6521451549, 0.3478548451 }, \
	{0.2369268851, 0.4786286705, 0.56 + 8 / 900.0, 0.4786286705, 0.2369268851}, \
	{0.1713244924, 0.3607615731, 0.4679139346, 0.4679139346, 0.3607615731, 0.1713244924}, \
	{0.1294849662, 0.2797053915, 0.3818300505, 0.4179591837, 0.3818300505, 0.2797053915, 0.1294849662}, \
	{0.1012285363, 0.2223810345, 0.3137066459, 0.3626837834, 0.3626837834, 0.3137066459, 0.2223810345, 0.1012285363}, \
	{0.0812743884, 0.1806481607, 0.2606106964, 0.3123470770, 0.3302393550, 0.3123470770, 0.2606106964, 0.1806481607, 0.0812743884}, \
	{0.0666713443, 0.1494513492, 0.2190863625, 0.2692667193, 0.2955242247, 0.2955242247, 0.2692667193, 0.2190863625, 0.1494513492, 0.0666713443 } };

// Set maximum divide count to avoid infinite loop for certain special conditions
//constexpr int MAX_DIVIDE = 16;	// This line will not compile in VS2013
static const int MAX_DIVIDE = 16;	// This line always works

template<int N> struct ODD_EVEN { enum { value = N%2 }; };

template<int N = 4>
double gl_integration(std::function<double(double)> func, double x_left, double x_right, double precision = 1e-6)
{
	// Because of Legendre function's symmetry, We only need half size of the tables.
	// Putting table into array could improve performance when division count is big. 
	std::array<double, (N + 1) / 2> nodes;
	std::array<double, (N + 1) / 2> coeffs;
	for (int i = 0; i < (N + 1) / 2; ++i)
	{
		nodes[i] = legendre_nodes[N-1][i];
		coeffs[i] = legendre_coeffs[N-1][i];
	}

	double result = 0;

	double step = (x_right - x_left)/2;
	double mid_point = x_left + step;
	int divide = 1;

	for (int i = 0; i < N / 2; ++i)
	{
		double offset = step * nodes[i];
		result += (func(mid_point + offset) + func(mid_point - offset)) * coeffs[i];
	}
	if (ODD_EVEN<N>::value)
		result += func(mid_point)*coeffs[(N - 1) / 2];
	result *= step;	// Get the initial value

	while (divide <= MAX_DIVIDE)
	{
		double temp_result = 0.0;
		divide *= 2;
		step /= 2;
		mid_point = x_left + step;	// Initial mid-point from left to right
		for (int i = 0; i < divide; ++i)
		{
			for (int j = 0; j < N / 2; ++j)
			{
				double offset = step * nodes[j];
				temp_result += (func(mid_point + offset) + func(mid_point - offset)) * coeffs[j];
			}
			if (ODD_EVEN<N>::value)
				temp_result += func(mid_point)*coeffs[(N - 1) / 2];
			// Adjust mid-point to next integration section
			mid_point += step;
			mid_point += step;
		}
		temp_result *= step;	// This time's result
		double error = abs((temp_result - result) / (1 + temp_result));	// Calculate error
		result = temp_result;
		if (error < precision)
			break;
	}
	//std::cout << divide << std::endl;

	if (divide > MAX_DIVIDE)
		throw result;			// If > MAX_DIVIDE, throw the final result to be processed from higher level.
	else
		return result;
}



int main(void)
{
	std::cout.precision(15);
	
	std::cout << gl_integration<2>([&](double x) { return x+1; }, -1, 3) << std::endl;
	std::cout << gl_integration([&](double x) { return x*x-5; }, 1, 3) << std::endl;

	std::cout << gl_integration<5>([&](double x) { return std::sin(x) / x; }, -3.1416, 3.1416) << std::endl;
	std::cout << gl_integration<6>([&](double x) { return std::sin(x) / x; }, -3.1416, 3.1416) << std::endl;

	return 0;
}


