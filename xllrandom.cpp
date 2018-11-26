// xllrandom.cpp - implement xllrandom.h classes
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllrandom.h"

using namespace xll;
/*
#ifdef _DEBUG
static AddInX xai_random(
	DocumentX(CATEGORY)
	.Documentation(R_(
This library calls the C++ <codeInline>&lt;random&gt;</codeInline> library.
The function <codeInline>RANDOM.ENGINE(Type, ?Seed)</codeInline> creates a 
random number generator <em>engine</em> of a given type with optional seed.
The types of engines available start with RANDOM_ENGINE_*.
\n
The function <codeInline>RANDOM.DISTRIBUTION(Type, ?Params...)</codeInline> creates
a random number <em>distribution</em> of a given type with optional parameters.
The types of distributions available start with RANDOM_DISTRIBUTION_*.
))
);
#endif // _DEBUG
*/
template<class T>
T random_variate(HANDLEX d, HANDLEX e)
{
	handle<distribution::base_distribution<T>> hd(d);
	handle<engine::base_engine<>> he(e);

	return (*hd)(*he);
}

static AddInX xai_random_bool(
	FunctionX(XLL_BOOL, _T("?xll_random_bool"), _T("RANDOM.BOOL"))
	.Arg(XLL_HANDLE, _T("Distribution"), _T("is a handle to a distribution returned by RANDOM.DISTRIBUTION."))
	.Arg(XLL_HANDLE, _T("Engine"), _T("is a handle to a random engine returned by RANDOM.ENGINE."))
	.Volatile()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns an boolean valued random variate."))
	.Documentation(
	)
);
BOOL WINAPI
xll_random_bool(HANDLEX dist, HANDLEX eng)
{
#pragma XLLEXPORT
	BOOL h{FALSE};

	try {
		h = random_variate<bool>(dist, eng);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return h;
	}

	return h;
}
static AddInX xai_random_int(
	FunctionX(XLL_DOUBLE, _T("?xll_random_int"), _T("RANDOM.INT"))
	.Arg(XLL_HANDLE, _T("Distribution"), _T("is a handle to a distribution returned by RANDOM.DISTRIBUTION."))
	.Arg(XLL_HANDLE, _T("Engine"), _T("is a handle to a random engine returned by RANDOM.ENGINE."))
	.Volatile()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns an integer valued random variate."))
	.Documentation(
	)
);
double WINAPI
xll_random_int(HANDLEX dist, HANDLEX eng)
{
#pragma XLLEXPORT
	double h{std::numeric_limits<double>::quiet_NaN()};

	try {
		h = random_variate<int>(dist, eng);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return h;
	}

	return h;
}
static AddInX xai_random_double(
	FunctionX(XLL_DOUBLE, _T("?xll_random_double"), _T("RANDOM.REAL"))
	.Arg(XLL_HANDLE, _T("Distribution"), _T("is a handle to a distribution returned by RANDOM.DISTRIBUTION."))
	.Arg(XLL_HANDLE, _T("Engine"), _T("is a handle to a random engine returned by RANDOM.ENGINE."))
	.Uncalced()
	.Volatile()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a floating point random variate. Defaults to uniform real on [0, 1)"))
	.Documentation(
	)
);
double WINAPI
xll_random_double(HANDLEX dist, HANDLEX eng)
{
#pragma XLLEXPORT
	double h{std::numeric_limits<double>::quiet_NaN()};

	try {
		static std::uniform_real_distribution<double> u(0,1);
		static handle<engine::base_engine<>> dre{new engine::base<std::default_random_engine>(engine::default_random())};

		if (!dist && !eng)
			h = u(*dre);
		else if (!eng)
			h = (*handle<distribution::base_distribution<double>>(dist))(*dre);
		else
			h = random_variate<double>(dist, eng);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return h;
	}

	return h;
}
#if 0

static AddInX xai_generate_variate(
	FunctionX(XLL_FPX XLL_VOLATILEX, _T("?xll_generate_variate"), _T("GENERATE.VARIATE"))
	.Arg(XLL_HANDLEX, _T("Generator"), _T("is a handle returned by VARIATE.GENERATOR."))
	.Arg(XLL_WORDX, _T("Rows?"), _T("is the number of rows of variates to generate."))
	.Arg(XLL_WORDX, _T("Columns?"), _T("is the number of columns of variates to generate. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Generate random variates. "))
	.Documentation(
		_T("This function generates random variates based on the engine and distribution ")
		_T("specified in the call to <codeInline>VARIATE.GENERATOR</codeInline>. ")
	)
);
_FP12* WINAPI
xll_generate_variate(HANDLEX vg, WORD r, WORD c)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		if (r <= 0)
			r = 1;
		if (c <= 0)
			c = 1;

		handle<variate::base_variate<double>> pvg(vg);
		ensure(pvg);

		x.reshape(r, c);
		for (WORD i = 0; i < x.size(); ++i)
			x[i] = (*pvg)(); // call virtual operator()
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return x.get();
}

//
// Generators
//

static AddInX xai_generator_mt19937(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_generator_mt19937"), _T("GENERATOR.MT19937"))
	.Arg(XLL_DOUBLEX, _T("Seed"), _T("is a 32-bit seed. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a Mersenne Twister generator."))
	.Documentation(
		_T("Mersenne Twister(MT) is a pseudorandom number generating algorithm developped by Makoto Matsumoto ")
		_T("and Takuji Nishimura (alphabetical order) in 1996/1997. An improvement on initialization was given on 2002 Jan. ")
		,
		xml::externalLink(_T("Mersenne Twistor home page"), _T("http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html"))
	)
);
HANDLEX WINAPI
xll_generator_mt19937(double seed)
{
#pragma XLLEXPORT
	return generator_engine<std::mt19937>(seed);
}

static AddInX xai_generator_ranlux64_base_01(
	FunctionX(XLL_HANDLEX, _T("?xll_generator_ranlux64_base_01"), _T("GENERATOR.RANLUX64.BASE.01"))
	.Arg(XLL_DOUBLEX, _T("Seed"), _T("is a seed. "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return handle to Knuth B generator. "))
	.Documentation()
);
HANDLEX WINAPI
xll_generator_ranlux64_base_01(double seed)
{
#pragma XLLEXPORT
	return generator_engine<std::ranlux64_base_01>(seed);
}

//
// Distributions
//
#if 0
static AddInX xai_distribution_gamma(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_gamma"), _T("DISTRIBUTION.GAMMA"))
	.Arg(XLL_DOUBLEX, _T("Alpha"), _T("is a the mean."))
	.Arg(XLL_DOUBLEX, _T("Beta"), _T("is a the standard deviation. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gamma distribution."))
	.Documentation(
		_T("The gamma distribution has mean <math>") ENT_alpha _T("/") ENT_beta _T("</math> and ")
		_T("variance <math>") ENT_alpha _T("/") ENT_beta _T("<superscript>2</superscript></math>. ")
	)
);
HANDLEX WINAPI
xll_distribution_gamma(double mu, double sigma)
{
#pragma XLLEXPORT
	handle<distribution::base_distribution<double>> h 
		= new distribution::type<std::gamma_distribution<double>>(mu, sigma);
	
	return h.get();
}

static AddInX xai_distribution_poisson(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_poisson"), _T("DISTRIBUTION.POISSON"))
	.Arg(XLL_DOUBLEX, _T("Lambda"), _T("is a the mean. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a poisson distribution."))
	.Documentation(
		_T("The poisson distribution was invented by Sim") ENT_eacute _T("on Denis Poisson to model the number of discrete ")
		_T("events that occur in an interval given a hazard rate, ") ENT_lambda _T(". ")
	)
);
HANDLEX WINAPI
xll_distribution_poisson(double lambda)
{
#pragma XLLEXPORT
	handle<distribution::base_distribution<double>> h 
		= new distribution::type<std::poisson_distribution<double>>(lambda);
	
	return h.get();
}
#endif // 0
static AddInX xai_distribution_normal(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_normal"), _T("DISTRIBUTION.NORMAL"))
	.Arg(XLL_DOUBLEX, _T("Mean"), _T("is a the mean."))
	.Arg(XLL_DOUBLEX, _T("Std"), _T("is a the standard deviation. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a normal distribution."))
	.Documentation(
		_T("The normal distribution was invented by Carl Friedrich Gauss to model phenomena that arise from ")
		_T("an accumulation of homogeneous, independent sources of randomness. ")
	)
);
HANDLEX WINAPI
xll_distribution_normal(double mu, double sigma)
{
#pragma XLLEXPORT
	handle<distribution::base_distribution<double>> h 
		= new distribution::type<std::normal_distribution<double>>(mu, sigma);
	
	return h.get();
}

static AddInX xai_distribution_uniform(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_uniform"), _T("DISTRIBUTION.UNIFORM"))
	.Arg(XLL_DOUBLEX, _T("Min"), _T("is a the smallest value generated."))
	.Arg(XLL_DOUBLEX, _T("Max"), _T("is a the strict upper bound. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a uniform distribution."))
	.Documentation(
		_T("Random variates between a minimum and maximum value uniformly distributed in that interval. ")
	)
);
HANDLEX WINAPI
xll_distribution_uniform(double min, double max)
{
#pragma XLLEXPORT
	handle<distribution::base_distribution<double>> h 
		= new distribution::type<std::uniform_real<double>>(min, max);
	
	return h.get();
}

static AddInX xai_distribution_poisson(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_poisson"), _T("DISTRIBUTION.POISSON"))
	.Arg(XLL_DOUBLEX, _T("Lambda"), _T("is the jump intensity. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a Poisson distribution."))
	.Documentation(
		_T(" ")
	)
);
HANDLEX WINAPI
xll_distribution_poisson(double lambda)
{
#pragma XLLEXPORT
	handlex h;

	try {
		ensure (lambda > 0);

		handle<distribution::base_distribution<double>>
			pd(new distribution::type<std::poisson_distribution<int>>(lambda));

		h = pd.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	
	return h;
}
#endif // 0

#ifdef _DEBUG
int
test_random(void)
{
	try {
		_crtBreakAlloc = 255;
		bool b = true;
		ensure (b);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<Open> xao_test_random(test_random);
#endif //