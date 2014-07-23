#if 0
// The first version
#include <iostream>
#include <vector>
#include <cstdint>

int mn_matrix_n(int m, int n)
{
	if (!m || !n)
		return 1;
	else
		return mn_matrix_n(m - 1, n) + mn_matrix_n(m, n - 1);
}

template<int M, int N>
struct MATRIX_PATH_CNT
{
	enum { val = MATRIX_PATH_CNT<M - 1, N>::val + MATRIX_PATH_CNT<M, N - 1>::val };
};

template<int N>
struct MATRIX_PATH_CNT<0, N>
{
	enum { val = 1 };
};

template<int M>
struct MATRIX_PATH_CNT<M, 0>
{
	enum { val = 1 };
};


int catalan_n(int m, int n)
{
	if (!m)
		return 1;
	else if (m == n)
		return catalan_n(m - 1, n);
	else if (m < n)
		return catalan_n(m - 1, n) + catalan_n(m, n - 1);
}

template<int M, int N>
struct M_GT_N
{
	enum{ val = M > N };
};

template<int M, int N, bool M_BIGGER_THEN_N>
struct CATALAN_NT
{
	enum { val = CATALAN_NT<M - 1, N, M_GT_N<M - 1, N>::val>::val + CATALAN_NT<M, N - 1, M_GT_N<M, N - 1>::val>::val };
};

template<int M, int N>
struct CATALAN_NT<M, N, true>
{
	enum { val = 0 };
};

template<int N>
struct CATALAN_NT<0, N, false>
{
	enum { val = 1 };
};


int combination_n(int m, int n)
{
	int result = 0;
	if (n == 1)
		return m;
	else
	{
		for (int i = 0; i < m - n + 1; ++i)
			result += combination_n(m - i - 1, n - 1);
	}
	return result;
}


template<int S, int M, int N> struct COMBO_NT_SUM;
template<int M, int N>
struct COMBO_NT
{
	enum { val = COMBO_NT_SUM<M - N+1, M, N>::val };
};
template<int M>
struct COMBO_NT<M, 1>
{
	enum { val = M };
};

template<int S, int M, int N>
struct COMBO_NT_SUM
{
	enum { val = COMBO_NT<M - S, N - 1>::val + COMBO_NT_SUM<S - 1, M, N>::val };
};
template<int M, int N>
struct COMBO_NT_SUM<0, M, N>
{
	enum { val = 0 };
};


template<int M, int N>
struct OTHER_COMBO
{
	enum {val = MATRIX_PATH_CNT<M-N, N>::val};
};


int permutation_n(int m, int n)
{
	int result = 0;
	if (n == 1)
		return m;
	else
	{
		for (int i = 0; i < m; ++i)
			result += permutation_n(m - 1, n - 1);
	}
	return result;
}

template<int S, int M, int N> struct PERMU_NT_SUM;
template<int M, int N>
struct PERMU_NT
{
	enum { val = PERMU_NT_SUM<M, M, N>::val };
};
template<int M>
struct PERMU_NT<M, 1>
{
	enum { val = M };
};

template<int S, int M, int N>
struct PERMU_NT_SUM
{
	enum { val = PERMU_NT<M - 1, N - 1>::val + PERMU_NT_SUM<S-1, M, N>::val };
};
template<int M, int N>
struct PERMU_NT_SUM<0, M, N>
{
	enum { val = 0 };
};

int permutation_n_new(int m, int n)
{
	if (n == 0 || m < n)
		return 1;
	else
		return m*permutation_n(m - 1, n - 1);
}

template<int M, int N>
struct PERMUTATION
{
	enum class v : int64_t {val = M*static_cast<int64_t>(PERMUTATION<M-1, N-1>::v::val)};
};
template<int M>
struct PERMUTATION<M, 0>
{
	enum class v : int64_t {val = 1};
};

std::vector<int> fib(int lmt)
{
	std::vector<int> fibs;
	int a = 1;
	int b = 1;
	while (b <= lmt)
	{
		fibs.push_back(b);
		b += a;
		a = b - a;
	}
	return fibs;
}

template<int N>
struct fibo
{
	enum { val = fibo<N - 1>::val + fibo<N - 2>::val };
};
template<>
struct fibo<0>
{
	enum { val = 1 };
};
template<>
struct fibo<1>
{
	enum { val = 1 };
};


int main(void)
{
	enum class x : int16_t {a = 1, b = 2};
	const int m = 10, n = 5;
	std::cout << "Combination of (" << m << ", " << n << ") is : " << combination_n(m, n) << std::endl;
	std::cout << "Permutation of (" << m << ", " << n << ") is : " << permutation_n(m, n) << ", " << permutation_n_new(m, n) << std::endl;
	std::vector<int> fibs = fib(1000);
	for (auto f : fibs)
		std::cout << f << "  ";
	std::cout << std::endl;

	std::cout << "The 20th Fibonacci number is: " << fibo<20>::val << std::endl;
	std::cout << "MATRIX_PATH_CNT(6, 3) is: " << MATRIX_PATH_CNT<3, 6>::val << std::endl;
	std::cout << "CATALAN_NT(16, 16) is: " << CATALAN_NT<16, 16, false>::val << std::endl;
	std::cout << "COMBO_NT(10, 4) is: " << COMBO_NT<10, 4>::val << ", " << OTHER_COMBO<10, 6>::val << ::std::endl;
	std::cout << "PERMUTATION(8, 5) is: " << PERMU_NT<13, 13>::val << ", " << static_cast<int64_t>(PERMUTATION<13, 13>::v::val) << std::endl;

	std::cout << static_cast<int16_t>(x::a) + static_cast<int16_t>(x::b) << std::endl;

	std::system("Pause");
	return 0;
}
#endif

#if 0
#include <iostream>
#include <cstdint>
#include <type_traits>

//
// Counting pathes from LU corner to RL corner of an M*N matrix
// Note that: 0*0 matrix will lead to ambiguity whilch can be solved by providing a MATRIX_PATH_CNT<0, 0> specialization
//
template<int M, int N> struct MATRIX_PATH_CNT
{
	enum class v : int64_t { val = static_cast<int64_t>(MATRIX_PATH_CNT<M, N-1>::v::val) + static_cast<int64_t>(MATRIX_PATH_CNT<M-1, N>::v::val) };
};
template<int M> struct MATRIX_PATH_CNT<M, 0>
{
	enum class v : int64_t { val = 1 };
};
template<int N> struct MATRIX_PATH_CNT<0, N>
{
	enum class v : int64_t { val = 1 };
};
template<> struct MATRIX_PATH_CNT<0, 0>
{
	enum class v : int64_t {val = 0 };
};
//
// Calculate the Nth Catalan number
//
template <int A, int B> struct LE
{
	enum { val = A<=B };
};
template<int M, int N, bool m_le_n> struct CATALAN
{
	enum class v : int64_t { val = static_cast<int64_t>(CATALAN<M-1, N, LE<M-1, N>::val>::v::val) + static_cast<int64_t>(CATALAN<M, N-1, LE<M, N-1>::val>::v::val) };
};
template<int M, int N> struct CATALAN<M, N, false>
{
	enum class v : int64_t { val = 0 };
};
template<int N> struct CATALAN<0, N, true>
{
	enum class v : int64_t { val = 1 };
};

//
// Calculate Combination number
//
template <int M, int N> struct COMBINATION
{
	enum class v : int64_t { val = static_cast<typename std::underlying_type<MATRIX_PATH_CNT<M, N>::v>::type>(MATRIX_PATH_CNT<M - N, N>::v::val) };
};

//
// Calculate Permutation number
//
template <int M, int N> struct PERMUTATION
{
	enum class v : int64_t { val = M * static_cast<int64_t>(PERMUTATION<M-1, N-1>::v::val) };
};
template<int M> struct PERMUTATION<M, 0>
{
	enum class v : int64_t { val = 1 };
};


int main(void)
{
	// This Matrix(0, 0) will lead to ambiguity, which can be solved by providing a MATRIX<0, 0> partial specialization
	//std::cout << "Matrix(0, 0) path count is : " << static_cast<int64_t>(MATRIX_PATH_CNT<0, 0>::v::val) << std::endl;
	std::cout << "Matrix(11, 9) path count is : " << static_cast<int64_t>(MATRIX_PATH_CNT<11, 9>::v::val) << std::endl;
	std::cout << "Catalan(25, 25) is : " << static_cast<int64_t>(CATALAN<25, 25, true>::v::val) << std::endl;
	//std::cout << "Catalan(5, 5) is : " << static_cast<int64_t>(CATALAN_A<5>::val) << std::endl;
	std::cout << "Combination(5, 3) is : " << static_cast<int64_t>(COMBINATION<5, 3>::v::val) << std::endl;
	std::cout << "Permutation(5, 3) is : " << static_cast<int64_t>(PERMUTATION<5, 3>::v::val) << std::endl;

	std::cout << "Combination(20, 11) is : " << static_cast<int64_t>(COMBINATION<20, 11>::v::val) << std::endl;
	std::cout << "Permutation(15, 15) is : " << static_cast<int64_t>(PERMUTATION<15, 15>::v::val) << std::endl;

	std::system("Pause");
	return 0;
}
#endif






#include <iostream>
#include <cstdint>
#include <limits>

/////////////////////////////////////////////////////////////////
//
//    General purpose template function to determine if M >= N
//
/////////////////////////////////////////////////////////////////
template<int M, int N> struct M_GT_N
{
	enum{ val = M > N };
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Counting pathes from LU corner to RL corner of an M*N matrix
//    Note that: 0*0 matrix will lead to ambiguity whilch can be solved by providing a MATRIX_PATH_CNT<0, 0> specialization
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int M, int N> struct MATRIX_PATH_CNT
{
	enum class v : int64_t { val = static_cast<int64_t>(MATRIX_PATH_CNT<M - 1, N>::v::val) + static_cast<int64_t>(MATRIX_PATH_CNT<M, N - 1>::v::val) };
};
template<int N> struct MATRIX_PATH_CNT<0, N>
{
	enum class v : int64_t { val = 1 };
};
template<int M> struct MATRIX_PATH_CNT<M, 0>
{
	enum class v : int64_t { val = 1 };
};
//////////////////////////////////////////////
//
//    Another way to calculate pathes
//
//////////////////////////////////////////////
template<int M, int N> struct COMBINATION;
template<int M, int N> struct OTHER_MATRIX_PATH_CNT
{
	enum class v : int64_t { val = static_cast<int64_t>(COMBINATION<M + N, N>::v::val) };
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Calculate Catalan Number
//
//////////////////////////////////////////////////////////////////////
template<int M, int N, bool flag> struct CATALAN
{
	enum class v : int64_t { val = static_cast<int64_t>(CATALAN<M - 1, N, M_GT_N<M - 1, N>::val>::v::val) + static_cast<int64_t>(CATALAN<M, N - 1, M_GT_N<M, N - 1>::val>::v::val) };
};
template<int M, int N> struct CATALAN<M, N, true>
{
	enum class v : int64_t { val = 0 };
};
template<int N> struct CATALAN<0, N, false>
{
	enum class v : int64_t { val = 1 };
};

//////////////////////////////////////////////////////////////////////
//
//    Another way to calculate Catalan Number
//
//////////////////////////////////////////////////////////////////////
template<int M, int I> struct CAT_SUM;
template<int M> struct OTHER_CATALAN
{
	enum class v : int64_t { val = static_cast<int64_t>(CAT_SUM<M - 1, M - 1>::v::val) };
};
template<> struct OTHER_CATALAN<0>
{
	enum class v : int64_t { val = 1 };
};
template<int M, int I> struct CAT_SUM
{
	enum class v : int64_t { val = static_cast<int64_t>(OTHER_CATALAN<M - I>::v::val) * static_cast<int64_t>(OTHER_CATALAN<I>::v::val) + static_cast<int64_t>(CAT_SUM<M, I - 1>::v::val) };
};
template<int M> struct CAT_SUM<M, -1>
{
	enum class v : int64_t { val = 0 };
};


//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Calculate Combination Number
//
//////////////////////////////////////////////////////////////////////
template<int S, int M, int N> struct COMBO_SUM;
template<int M, int N> struct COMBINATION
{
	enum class v : int64_t { val = static_cast<int64_t>(COMBO_SUM<M - N + 1, M, N>::v::val) };
};
template<int M> struct COMBINATION<M, 1>
{
	enum class v : int64_t { val = M };
};
template<int S, int M, int N> struct COMBO_SUM
{
	enum class v : int64_t { val = static_cast<int64_t>(COMBINATION<M - S, N - 1>::v::val) + static_cast<int64_t>(COMBO_SUM<S - 1, M, N>::v::val) };
};
template<int M, int N> struct COMBO_SUM<0, M, N>
{
	enum class v : int64_t { val = 0 };
};

//////////////////////////////////////////////////////////////////////
//
//    Another way to calculate Combination Number
//
//////////////////////////////////////////////////////////////////////
template<int M, int N> struct OTHER_COMBO
{
	enum class v : int64_t { val = MATRIX_PATH_CNT<M - N, N>::v::val };
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Calculate Permutation Number
//
//////////////////////////////////////////////////////////////////////
template<int S, int M, int N> struct PERMU_SUM;
template<int M, int N> struct PERMUTATION
{
	enum class v : int64_t { val = static_cast<int64_t>(PERMU_SUM<M, M, N>::v::val) };
};
template<int M> struct PERMUTATION<M, 1>
{
	enum class v : int64_t { val = M };
};
template<int S, int M, int N> struct PERMU_SUM
{
	enum class v : int64_t { val = static_cast<int64_t>(PERMUTATION<M - 1, N - 1>::v::val) + static_cast<int64_t>(PERMU_SUM<S - 1, M, N>::v::val) };
};
template<int M, int N> struct PERMU_SUM<0, M, N>
{
	enum class v : int64_t { val = 0 };
};

//////////////////////////////////////////////////////////////////////
//
//    Another way to calculate Permutation Number
//
//////////////////////////////////////////////////////////////////////
template<int M, int N> struct OTHER_PERMU
{
	enum class v : int64_t { val = M*static_cast<int64_t>(OTHER_PERMU<M - 1, N - 1>::v::val) };
};
template<int M> struct OTHER_PERMU<M, 0>
{
	enum class v : int64_t { val = 1 };
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Calculate the N-th Fibonacci Number
//
//////////////////////////////////////////////////////////////////////
template<int N> struct FIBONACCI
{
	enum class v : int64_t { val = static_cast<int64_t>(FIBONACCI<N - 1>::v::val) + static_cast<int64_t>(FIBONACCI<N - 2>::v::val) };
};
template<> struct FIBONACCI<1>
{
	enum class v : int64_t { val = 1 };
};
template<> struct FIBONACCI<0>
{
	enum class v : int64_t { val = 1 };
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Calculate N!
//
//////////////////////////////////////////////////////////////////////
template<int N> struct FACTORIAL
{
	enum class v : int64_t { val = N * static_cast<int64_t>(FACTORIAL<N-1>::v::val) };
};
template<> struct FACTORIAL<0>
{
	enum class v { val = 1 };
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Calcualte the sum from M to N
//
//////////////////////////////////////////////////////////////////////
template<int X, int Y, bool X_GT_Y = M_GT_N<X, Y>::val> struct ACCUMULATE
{
	enum class v : int64_t { val = 0 };
};
template<int X, int Y> struct ACCUMULATE<X, Y, false>
{
	enum class v : int64_t { val = X + static_cast<int64_t>(ACCUMULATE<X + 1, Y, false>::v::val) };
};
template<int X, int Y> struct ACCUMULATE<X, Y, true>
{
	enum class v : int64_t { val = X + static_cast<int64_t>(ACCUMULATE<X-1, Y, true>::v::val) };
};
template<int Y> struct ACCUMULATE<Y, Y, false>
{
	enum class v { val = Y };
};
template<int Y> struct ACCUMULATE<Y, Y, true>
{
	enum class v { val = Y };
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Find the Greatest Common Divisor of two integers
//
//////////////////////////////////////////////////////////////////////
template<int X, int Y, bool X_GT_Y=M_GT_N<X, Y>::val> struct FIND_GCD
{
	enum {val = 1};
};
template<int X, int Y> struct FIND_GCD<X, Y, false>
{
	enum {val = FIND_GCD<Y%X, X, false>::val};
};
template<int Y> struct FIND_GCD<0, Y, false>
{
	enum {val = Y};
};

template<int X, int Y> struct FIND_GCD<X, Y, true>
{
	enum {val = FIND_GCD<Y, X%Y, true>::val};
};
template<int X> struct FIND_GCD<X, 0, true>
{
	enum {val = X};
};



//------------------------------------------- Seperator Line ---------------------------------------------------------------



//////////////////////////////////////////////////////////////////////
//
//    Find the Greatest Common Divisor of two integers
//
//////////////////////////////////////////////////////////////////////
template<int N> struct N_BITS { enum {val = 1 + N_BITS<N/2>::val}; };
template<> struct N_BITS<0> { enum {val = 0}; };



int main(void)
{
	std::cout << "Maximum value of int64_t is: " << std::numeric_limits<int64_t>::max() << std::endl;
	std::cout << "Maximum value of long long is: " << std::numeric_limits<long long>::max() << std::endl << std::endl;

	std::cout << "MATRIX_PATH_CNT(6, 3) is: " << static_cast<int64_t>(MATRIX_PATH_CNT<3, 6>::v::val) << ", " << static_cast<int64_t>(OTHER_MATRIX_PATH_CNT<6, 3>::v::val) << std::endl;
	std::cout << "CATALAN(20, 20) is: " << static_cast<int64_t>(CATALAN<20, 20, false>::v::val) << ", " << static_cast<int64_t>(OTHER_CATALAN<20>::v::val) << std::endl;

	std::cout << "COMBINATION(10, 4) is: " << static_cast<int64_t>(COMBINATION<10, 4>::v::val) << ", " << static_cast<int64_t>(OTHER_COMBO<10, 6>::v::val) << ", " \
		<< static_cast<int64_t>(FACTORIAL<10>::v::val) / (static_cast<int64_t>(FACTORIAL<4>::v::val) * static_cast<int64_t>(FACTORIAL<10-4>::v::val)) << std::endl;

	std::cout << "PERMUTATION(15, 13) is: " << static_cast<int64_t>(PERMUTATION<15, 13>::v::val) << ", " << static_cast<int64_t>(OTHER_PERMU<15, 13>::v::val) << ", " \
		<< static_cast<int64_t>(FACTORIAL<15>::v::val) / static_cast<int64_t>(FACTORIAL<15-13>::v::val) << std::endl;

	std::cout << "Fibonacci(50) is : " << static_cast<int64_t>(FIBONACCI<50>::v::val) << std::endl;
	std::cout << "Factorial(13) is : " << static_cast<int64_t>(FACTORIAL<21>::v::val) <<std::endl;
	std::cout << "Accumulation(3, 100) is : " << static_cast<int64_t>(ACCUMULATE<3, 100>::v::val) << std::endl;
	std::cout << "Accumulation(100, 3) is : " << static_cast<int64_t>(ACCUMULATE<100, 3>::v::val) << std::endl;
	
	std::cout << "Greatest Common Divisor of " << 13*5 << " and " << 13*7 << " is: " << FIND_GCD<13*5, 13*7>::val << std::endl;
	std::cout << "Greatest Common Divisor of " << 13*7 << " and " << 13*5 << " is: " << FIND_GCD<13*7, 13*5>::val << std::endl;

	std::cout << "Greatest Common Divisor of " << 97*13 << " and " << 97*41 << " is: " << FIND_GCD<97*13, 97*41>::val << std::endl;
	std::cout << "Greatest Common Divisor of " << 97*41 << " and " << 97*13 << " is: " << FIND_GCD<97*41, 97*13>::val << std::endl;

	std::cout << "Number 1023, 1024, 1025 must have at least " << N_BITS<1023>::val << ", " << N_BITS<1024>::val << ", " << N_BITS<1025>::val << " bits to represent them respectively." << std::endl;

	return 0;
}

