// xlldistribution.cpp - distribution functions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "tukey.h"
#include "xllrandom.h"

#define UNPAREN(...) __VA_ARGS__

#define DISTRIBUTION(X) \
X(BERNOULLI, UNPAREN(bernoulli_distribution), bool, UNPAREN(double), UNPAREN(p), "Return true with probability p, false with probability 1 - p") \
X(BINOMIAL, UNPAREN(binomial_distribution<int,double>), int, UNPAREN(int,double), UNPAREN(t,p), "Return i with probability C(t,i) p^i (1 - p)^(t - i)") \
X(CAUCHY, UNPAREN(cauchy_distribution<double>), double, UNPAREN(double,double), UNPAREN(a,b), "Density 1/pi*(1 + x^2)") \
X(CHI_SQUARED, UNPAREN(chi_squared_distribution<double>), double, UNPAREN(double), UNPAREN(n), "Sum of the squares of n standard normal random variables") \
X(DISCRETE, UNPAREN(discrete_distribution<int>), int, UNPAREN(std::initializer_list<double>), UNPAREN(probabilities), "Return i with probability p[i]") \
X(EXPONENTIAL, UNPAREN(exponential_distribution<double>), double, UNPAREN(double), UNPAREN(lambda), "Cumulative distribution 1 - exp(-lambda x), x > 0") \
X(EXTREME_VALUE, UNPAREN(extreme_value_distribution<double>), double, UNPAREN(double,double), UNPAREN(a,b), "Cumulative distribution exp(-exp(-x))") \
X(FISHER_F, UNPAREN(fisher_f_distribution<double>), double, UNPAREN(double,double), UNPAREN(m,n), "Quotient of chi squared distributions") \
X(GAMMA, UNPAREN(gamma_distribution<double>), double, UNPAREN(double,double), UNPAREN(alpha,beta), "Density x^alpha exp(-x/beta)/Gamma(alpha)beta^alpha, x > 0") \
X(GEOMETRIC, UNPAREN(geometric_distribution<int>), int, UNPAREN(double), UNPAREN(p), "Return i with probability p (1 - p)^i") \
X(LOGNORMAL, UNPAREN(lognormal_distribution<double>), double, UNPAREN(double,double), UNPAREN(m,s), "Exponential of normal distribution") \
X(NEGATIVE_BINOMIAL, UNPAREN(negative_binomial_distribution<int>), int, UNPAREN(int,double), UNPAREN(k,p), "") \
X(NORMAL, UNPAREN(normal_distribution<double>), double, UNPAREN(double,double), UNPAREN(mean,stddev), "Density exp(-x^2/2)/sqrt(2 pi)") \
X(PIECEWISE_CONSTANT, UNPAREN(piecewise_constant_distribution<double>), double, UNPAREN(std::vector<double>), UNPAREN(intervals,densities), "") \
X(PIECEWISE_LINEAR, UNPAREN(piecewise_linear_distribution<double>), double, UNPAREN(std::vector<double>), UNPAREN(intervals,densities), "") \
X(POISSON, UNPAREN(poisson_distribution<int>), int, UNPAREN(double), UNPAREN(mean), "Return i with probability m^i exp(-m)/i!") \
X(STUDENT_T, UNPAREN(student_t_distribution<double>), double, UNPAREN(double), UNPAREN(n), "Density proportional to (1 + x^2/n)^(-(n+1)/2)") \
X(UNIFORM_REAL, UNPAREN(uniform_real_distribution<double>), double, UNPAREN(double,double), UNPAREN(a,b), "Uniform reals on [a,b)") \
X(WEIBULL, UNPAREN(weibull_distribution<double>), double, UNPAREN(double,double), UNPAREN(a,b), "Density a/b (x/b)^(a-1) exp(-(x/b)^a), x > 0") \
X(TUKEY, UNPAREN(tukey_lambda_distribution<double>), double, UNPAREN(double), UNPAREN(lambda), "Quantile (q^lambda - (1-q)^lambda)/lambda") \
//X(UNIFORM_INT, UNPAREN(uniform_int_distribution<int>), int, UNPAREN(int,int), UNPAREN(a,b), "Uniform integers on [a,b]") \
// illegal call of non-static member function

#define ENUM_(a,b,c,d,e,f) RANDOM_DISTRIBUTION_ ## a,
enum Distribution { DISTRIBUTION(ENUM_) };

#define HASH_(x) #x
#define XLL_ENUM_(a,b,c,d,e,f) XLL_ENUM(RANDOM_DISTRIBUTION_##a, RANDOM_DISTRIBUTION_##a, "Random", f ". Parameters: " HASH_(UNPAREN(e)))
DISTRIBUTION(XLL_ENUM_)

using namespace xll;

static AddInX xai_random_distribution(
	FunctionX(XLL_HANDLEX, _T("?xll_random_distribution"), _T("RANDOM.DISTRIBUTION"))
	.Arg(XLL_USHORTX, _T("Type"), _T("is a the type of distribution from RANDOM_DISTRIBUTION_*."),
		_T("=RANDOM_DISTRIBUTION_UNIFORM_REAL()"))
	.Arg(XLL_LPOPERX, _T("?Params"), _T("are optional parameters to the distribution."),
		_T("={0,1}"))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a distribution with given Type."))
	.Documentation(
	)
);
HANDLEX WINAPI
xll_random_distribution(USHORT type, LPOPERX px)
{
#pragma XLLEXPORT
	handlex h;

	try {
		switch (type) {
#define CASE_(a,b,c,d,e,f) case RANDOM_DISTRIBUTION_ ## a: { \
			ensure (px->size() >= (std::tuple_size<std::tuple<d>>::value)); \
			handle<distribution::base_distribution<c>> hd = \
				new distribution::base<c,std:: ## b, d>(*px); \
			h = hd.get(); break; }

			DISTRIBUTION(CASE_)

		default:
			throw std::runtime_error("RANDOM.DISTRIBUTION: unknown distribution type");
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	
	return h;
}

static AddInX xai_random_distribution_min(
	FunctionX(XLL_DOUBLEX, _T("?xll_random_distribution_min"), _T("RANDOM.DISTRIBUTION.MIN"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a distribution from RANDOM.DISTRIBUTION.*."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the minimum value a distribution can take."))
	.Documentation(
	)
);
double WINAPI
xll_random_distribution_min(HANDLEX dist)
{
#pragma XLLEXPORT
	try {
		handle<distribution::base_distribution<double>> hd(dist, false);
		if (hd)
			return hd->min();

		handle<distribution::base_distribution<int>> hi(dist);
		if (hi)
			return hi->min();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	
	return std::numeric_limits<double>::quiet_NaN();
}
static AddInX xai_random_distribution_max(
	FunctionX(XLL_DOUBLEX, _T("?xll_random_distribution_max"), _T("RANDOM.DISTRIBUTION.MAX"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a distribution from RANDOM.DISTRIBUTION.*."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the maximum value a distribution can take."))
	.Documentation(
	)
);
double WINAPI
xll_random_distribution_max(HANDLEX dist)
{
#pragma XLLEXPORT
	try {
		handle<distribution::base_distribution<double>> hd(dist, false);
		if (hd)
			return hd->max();

		handle<distribution::base_distribution<int>> hi(dist);
		if (hi)
			return hi->max();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	
	return std::numeric_limits<double>::quiet_NaN();
}

#ifdef _DEBUG

int xll_test_random_distribution(void)
{
	try {
		OPERX p{OPERX(0),OPERX(10)};

		HANDLEX h = xll_random_distribution(RANDOM_DISTRIBUTION_UNIFORM_REAL, &p);
		handle<distribution::base_distribution<double>> hd(h);
		ensure (hd->min() == p[0]);
		ensure (hd->max() == p[1]);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}
static Auto<OpenAfter> xao_test_random_distribution(xll_test_random_distribution);

#endif // _DEBUG