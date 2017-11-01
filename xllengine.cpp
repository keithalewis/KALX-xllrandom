// xllengine.cpp - rng engines
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllrandom.h"

#define ENGINE(X) \
X(DEFAULT, default_random_engine, "Defaults to mt19937") \
X(KNUTH_B, knuth_b, "Shuffle order engine based on minstd_rand0") \
X(MINSTD_RAND, minstd_rand, "Generates a random sequence by the linear congruential algorithm.") \
X(MINSTD_RAND0, minstd_rand0, "Generates a random sequence by the linear congruential algorithm.") \
X(MT19937, mt19937, "Generates a high quality random sequence of integers based on the Mersenne twister algorithm.") \
X(MT19937_64, mt19937_64, "Generates a high quality random sequence of integers based on the Mersenne twister algorithm.") \
//X(RANLUX24, ranlux24, "Generates a random sequence using discard block and subtract-with-carry (lagged Fibonacci) algorithm.") \
//X(RANLUX3, ranlux3, "Generates a random sequence using discard block and subtract-with-carry (lagged Fibonacci) algorithm.") \
//X(RANLUX4, ranlux4, "Generates a random sequence using discard block and subtract-with-carry (lagged Fibonacci) algorithm.") \
//X(RANLUX48, ranlux48, "Generates a random sequence using discard block and subtract-with-carry (lagged Fibonacci) algorithm.") \

#define ENUM_(a,b,c) RANDOM_ENGINE_ ## a,
enum Engine { ENGINE(ENUM_) };

#define XLL_ENUM_(a,b,c) XLL_ENUM(RANDOM_ENGINE_##a, RANDOM_ENGINE_##a, "Random", c)
ENGINE(XLL_ENUM_)

using namespace xll;

static AddInX xai_random_engine(
	FunctionX(XLL_HANDLEX, _T("?xll_random_engine"), _T("RANDOM.ENGINE"))
	.Arg(XLL_USHORTX, _T("Type"), _T("is an enumeration from RANDOM_ENGINE_*."))
	.Arg(XLL_LPOPERX, _T("?Seed"), _T("is an optional array of numbers to seed the Engine."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a handle to a random variate generator of type Engine."))
	.Documentation(
	)
);
#pragma warning(push)
#pragma warning(disable: 4244)
HANDLEX WINAPI
xll_random_engine(USHORT eng, LPOPERX pseed)
{
#pragma XLLEXPORT
	handlex h;

	try {
		engine::seed ss{*pseed}, *pss{&ss};

		const OPERX& s((*pseed)[0]);
		if (pseed->size() == 1 && s.xltype == xltypeNum) {
			handle<engine::seed> hs(s.val.num, false);
			if (hs)
				pss = hs;
		}

		switch (eng) {
#define CASE_(a,b,c) case RANDOM_ENGINE_ ## a: { \
		handle<engine::base_engine<>> he(new engine::base<std:: ## b>(*pss)); \
		h = he.get(); break;}

		ENGINE(CASE_)
		
		default:
			throw std::runtime_error("RANDOM.ENGINE: unknown engine type");
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return h;
	}

	return h;
}
#pragma warning(pop)

static AddInX xai_random_engine_min(
	FunctionX(XLL_DOUBLEX, _T("?xll_random_engine_min"), _T("RANDOM.ENGINE.MIN"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a random engine returned by RANDOM.ENGINE."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the minimum value returned by the engine corresponding to Handle."))
	.Documentation(
	)
);
double WINAPI
xll_random_engine_min(HANDLEX eng)
{
#pragma XLLEXPORT
	double h{std::numeric_limits<double>::max()};

	try {
		handle<engine::base_engine<>> he(eng);

		h = he->min();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return h;
	}

	return h;
}
static AddInX xai_random_engine_max(
	FunctionX(XLL_DOUBLEX, _T("?xll_random_engine_max"), _T("RANDOM.ENGINE.MAX"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a random engine returned by RANDOM.ENGINE."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the maximum value returned by the engine corresponding to Handle."))
	.Documentation(
	)
);
double WINAPI
xll_random_engine_max(HANDLEX eng)
{
#pragma XLLEXPORT
	double h{std::numeric_limits<double>::max()};

	try {
		handle<engine::base_engine<>> he(eng);

		h = he->max();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return h;
	}

	return h;
}

