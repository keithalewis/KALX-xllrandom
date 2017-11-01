// xllrandom.cpp - Rename this file and replace this description.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "xllrandom.h"

#ifndef CATEGORY
#define CATEGORY _T("Random")
#endif

using namespace std;
using namespace xll;

// save seed for reset
static generator::seed<mt19937> s(random::default_random());
// standard normal
static auto normal = std::normal_distribution<double>();

static AddInX xai_random_brownian(
	FunctionX(XLL_FPX XLL_VOLATILEX, _T("?xll_random_brownian"), _T("RANDOM.BROWNIAN"))
	.Arg(XLL_FPX, _T("Times"), _T("is an array of times at which to sample Brownian motion"))
	.Arg(XLL_DOUBLEX, _T("Mu"), _T("is the drift rate of the Brownian Motion"))
	.Arg(XLL_DOUBLEX, _T("Sigma"), _T("is the standard deviation at time 1"))
	.Arg(XLL_BOOLX, _T("Reset"), _T("is a boolean seed for reseting the random seed. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a sample path at Times with drift Mu and standard deviation Sigma"))
	.Documentation(
		_T("The standard deviation corresponds to the sample at time 1. ")
		_T("The expected value at time <math>t</math> is <math>") ENT_mu _T("</math> and the ")
		_T("standard deviation at time <math>t</math> is <math>") ENT_sigma ENT_radic _T("</math>. ")
	)
);
xfp* WINAPI
xll_random_brownian(xfp* pt, double mu, double sigma, BOOL reset)
{
#pragma XLLEXPORT

	try {
		if (sigma == 0)
			sigma = 1;

		if (reset) {
			s.load(random::engine);
		}

		double t0 = pt->array[0];
		ensure (t0 >= 0);
		if (t0 > 0)
			pt->array[0] = mu*pt->array[0] + sigma*sqrt(t0)*normal(random::default_random());
		else
			pt->array[0] = mu*pt->array[0];

		for (xword i = 1; i < size(*pt); ++i) {
			double t = pt->array[i];
			ensure (t > t0);
			pt->array[i] = pt->array[i - 1] + mu*(t - t0) + sigma*sqrt(t - t0)*normal(random::default_random());
			t0 = t;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return pt;
}
