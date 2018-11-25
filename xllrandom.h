// xllrandom.h - random number generation and distributions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
// Uncomment the following line to use features for Excel2007 and above.
//#define EXCEL12
#pragma once
#include <random>
#include "xll12/xll/xll.h"

#ifndef CATEGORY
#define CATEGORY L"Random"
#endif

namespace random {

    inline std::default_random_engine dre;

    // random engine interface
    struct variate {
        void fill(size_t n, LPXLOPER12 px)
        {
            _fill(n, px);
        }
    private:
        virtual void _fill(size_t n, LPXLOPER12 px) = 0;
    };

    template<class R>
    struct uniform_real_variate : public variate {
        std::uniform_real_distribution<double> u;
        R& r;
        uniform_real_variate(std::uniform_real_distribution<double> u, R& r)
            : u(u), r(r)
        { }
        void _fill(size_t n, LPXLOPER12 px) override
        {
            while (n--) {
                px->xltype = xltypeNum;
                px->val.num = u(r);
                ++px;
            }
        }
    };

} // namespace random


