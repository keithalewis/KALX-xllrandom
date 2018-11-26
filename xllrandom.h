// xllrandom.h - random number generation and distributions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
// Uncomment the following line to use features for Excel2007 and above.
//#define EXCEL12
#pragma once
#define NOMINMAX
#include <sstream>
#include <random>
#include <utility>
#include "../xll12/xll/xll.h"

#undef _T
#define _T(x) L##x

#ifndef CATEGORY
#define CATEGORY _T("Random")
#endif


