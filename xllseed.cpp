// xllseed.cpp - seed related functions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllrandom.h"

using namespace xll;

static AddInX xai_seed_seq(
	FunctionX(XLL_HANDLE, _T("?xll_seed_seq"), _T("RANDOM.SEED.SEQ"))
	.Arg(XLL_LPOPER, _T("Seed"), _T("is an array of 32-bit unsigned numbers. "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a std::seed_seq. "))
	.Documentation(
		_T("This can be used as an argument to the <codeInline>RANDOM.ENGINE</codeInline> functions. ")
	)
);
HANDLEX WINAPI
xll_seed_seq(const LPOPER ps)
{
	handlex h;
#pragma XLLEXPORT
	try {
		handle<engine::seed> hs(new engine::seed(*ps));
		h = hs.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_seed_seq_generate(
	FunctionX(XLL_FP, _T("?xll_seed_seq_generate"), _T("RANDOM.SEED.SEQ.GENERATE"))
	.Arg(XLL_HANDLE, _T("Handle"), _T("is a handle returned by RANDOM.SEED.SEQ."))
	.Arg(XLL_WORD, _T("Count"), _T("is the number of new seeds to generate. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Generate new seeds from old seeds. "))
	.Documentation(
	)
);
_FP12* WINAPI
xll_seed_seq_generate(HANDLEX h, WORD n)
{
#pragma XLLEXPORT
	static FPX s;

	try {
		handle<engine::seed> h_(h);
		ensure (h_);

		if (n == 0)
			n = 1;

		std::vector<unsigned int> ss(n);
		h_->generate(ss.begin(), ss.end()); //!!! operator<< does not work on doubles

		s.reshape(n, 1);
		std::copy(ss.begin(), ss.end(), s.begin());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return s.get();
}

static AddInX xai_seed_seq_param(
	FunctionX(XLL_FP, _T("?xll_seed_seq_param"), _T("RANDOM.SEED.SEQ.PARAM"))
	.Arg(XLL_HANDLE, _T("Handle"), _T("is a handle returned by SEED.SEQ. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the seed sequence as a one column array. "))
	.Documentation(
		_T("The return value can be used as an argument to <codeInline>RANDOM.SEED.SEQ</codeInline>. ")
	)
);
_FP12* WINAPI
xll_seed_seq_param(HANDLEX h)
{
#pragma XLLEXPORT
	static xll::FP12 s;

	try {
		handle<engine::seed> h_(h);

		s.resize(static_cast<WORD>(h_->size()), 1);
		h_->param(s.begin());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return s.get();
}

