// tukey.h - Tukey lambda distributions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include <random>

namespace std {

	template<class T = double>
	class tukey_lambda_distribution {
	public:
		typedef T result_type;

		struct param_type {
			T lambda_;
			param_type(T lambda)
				: lambda_(lambda)
			{ }
			bool operator==(const param_type& pt)
			{
				return lambda_ == pt.lambda_;
			}
			bool operator!=(const param_type& pt)
			{
				return !operator==(pt);
			}
			T lambda() const
			{
				return lambda_;
			}
		};
		explicit tukey_lambda_distribution(T lambda = T(0))
			: pt_(lambda)
		{ }
		explicit tukey_lambda_distribution(const param_type& pt)
			: pt_(pt)
		{ }
		T lambda() const
		{
			return pt_.lambda();
		}
		T (min)() const
		{
			return -std::numeric_limits<T>::max();
		}
		T (max)() const
		{
			return std::numeric_limits<T>::max();
		}
		void reset()
		{ }
		template<class E>
		T operator()(E& e)
		{
			return eval_(e, pt_.lambda());
		}
		template<class E>
		T operator()(E& e, const param_type& pt)
		{
			return eval_(e, pt);
		}
	private:
		template<class E>
		T eval_(E& e, const param_type& pt)
		{
			static std::uniform_real_distribution<T> u;

			T q = u(e);
			T l = pt.lambda();

			return 1 + l == 1 ? log(q)/(1 - q) : (std::pow(q,l) - std::pow(1 - q,l))/l;
		}
		param_type pt_;
	};
/*
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
*/
};