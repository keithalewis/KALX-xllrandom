// distribution.h - extended C++ distributions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include <random>

namespace distribution {

	template<class T>
	class tukey_lambda {
		T lambda_;
	public:
		typedef T input_type;
		typedef T result_type;

		explicit tukey_lambda(T lambda = T(0))
			: lambda_(lambda)
		{ }
		T lambda() const
		{
			return lambda_;
		}
		T (min)() const
		{
			return 0;
		}
		T (max)() const
		{
			return 0;
		}
		void reset()
		{ }
		template<class E>
		T operator()(E& e)
		{
			static std::uniform_real_distribution<T> u;

			T q = u(e);

			return 1 + lambda_ == 1 ? log(q)/(1 - q) : (std::pow(q,lambda_) - std::pow(1 - q,lambda_))/lambda_;
		}
	};

	template<class T>
	class generalized_lambda {
		T lambda1_, lambda2_, lambda3_, lambda4_;
	public:
		explicit generalized_lambda(T lambda1 = T(0), T lambda2 = T(1), T lambda3 = T(0), T lambda4 = T(0))
			: lambda1_(lambda1), lambda2_(lambda2), lambda3_(lambda3), lambda4_(lambda4)
		{ }
		T operator()(E& e)
		{
			static std::uniform_real_distribution<T> u;

			T q = u(e);

			return lambda1_ + (std::pow(q,lambda3_) - std::pow(1 - q, lambda4_))/lambda2_;
		}
	};
};