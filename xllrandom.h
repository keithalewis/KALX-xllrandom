// xllrandom.h - random number generation and distributions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
// Uncomment the following line to use features for Excel2007 and above.
//#define EXCEL12
#pragma once
#define NOMINMAX
#include <sstream>
#include <random>
#include <utility>
#include "xll/xll.h"

#ifndef CATEGORY
#define CATEGORY _T("Random")
#endif

typedef xll::traits<XLOPERX>::xfp xfp;
typedef xll::traits<XLOPERX>::xword xword;

namespace engine {

	class seed : public std::seed_seq {
		std::string s_;
	public:
		using std::seed_seq::seed_seq;
		using std::seed_seq::generate;
		seed()
			: std::seed_seq{}
		{ }
		seed(const std::vector<unsigned>& s)
			: std::seed_seq(s.begin(), s.end())
		{ }
		template<class X>
		seed(const XOPER<X>& s)
			: seed(seq(s))
		{
		}
		seed(const seed&) = delete;
		seed& operator=(const seed&) = delete;
		~seed()
		{ }
	private:
		template<class X> 
		static std::vector<unsigned> seq(const XOPER<X>& o)
		{
			if (o.xltype == xltypeNum)
				return std::vector<unsigned>(1, static_cast<unsigned>(o.val.num));

			if (o.xltype != xltypeMulti)
				return std::vector<unsigned>{};

			std::vector<unsigned> s(o.size());

			std::transform(o.begin(), o.end(), s.begin(), [](const XOPER<X>& o) {
				ensure (o.xltype == xltypeNum);
				return static_cast<unsigned>(o.val.num);
			});

			return s;
		}
	};

	// common base class for engines
	template<class T = unsigned int>
	struct base_engine {
		typedef T result_type;
		virtual ~base_engine() { }

		// forward to pure virtual functions in derived class
		result_type operator()()
		{
			return _op();
		}
		void seed(const std::seed_seq& s)
		{
			return _seed(s);
		}
		result_type min() const
		{
			return _min();
		}
		result_type max() const
		{
			return _max();
		}
		void seed(result_type s)
		{
			return _seed(s);
		}
		void discard(unsigned long long n)
		{
			return _discard(n);
		}
	private:
		virtual result_type _op() = 0;
		virtual void _seed(const std::seed_seq&) = 0;
		virtual result_type _min() const = 0;
		virtual result_type _max() const = 0;
		virtual void _seed(result_type) = 0;
		virtual void _discard(unsigned long long) = 0;
	};

	template<class E, class T = unsigned int>
	struct base : public base_engine<T>, public E {
		typedef T result_type;
		base()
			: E()
		{ }
		base(result_type seed)
			: E(seed)
		{ }
		base(const std::seed_seq& seed)
			: E(seed)
		{ }

		// implement pure virtual functions
		result_type _op()
		{
			return E::operator()();
		}
		void _seed(const std::seed_seq& s)
		{
			return E::seed(s);
		}
		result_type _min() const
		{
			return E::min();
		}
		result_type _max() const
		{
			return E::max();
		}
		void _seed(result_type s)
		{
			E::seed(s);
		}
		void _discard(unsigned long long n)
		{
			E::discard(n);
		}
	};

	// singleton for default rng.
	inline base<std::default_random_engine>& default_random(void)
	{
		static base<std::default_random_engine> e{};

		return e;
	}

} // namespace engine

namespace distribution {

	// common base class for distributions
	template<class T>
	struct base_distribution {
		typedef T result_type;
		base_distribution()
		{ }
		virtual ~base_distribution() { }

		// forward to pure virtual function
		T operator()(engine::base_engine<>& e)
		{
			return _op(e);
		}
		T (min)() const
		{
			return _min();
		}
		T (max)() const
		{
			return _max();
		}
	private:
		virtual T _op(engine::base_engine<>&) = 0;
		virtual T _min() const = 0;
		virtual T _max() const = 0;
	};

	// until we get std::integer_sequence
	// http://stackoverflow.com/questions/7858817/
	template<xword... i>
	struct indices {};

	// e.g., mi<3> -> mi<2,2> -> mi<1,1,2> -> mi<0,0,1,2>
	template<xword n, xword... i>
	struct make_indices : make_indices<n-1, n-1, i...> {};

	// specialize mi<0,0,1,2> => mi<0,1,2>
	template<xword... i>
	struct make_indices<0, i...> {
		typedef indices<i...> type;
	};

	// <result_Type, Dist, Params>
	template<class T, class D, class... Ps> 
	struct base : public base_distribution<T>, public D {
		base(const OPERX& o)
			: D(param<Ps...>(o))
		{ }
		template<class... Ps>
		typename D::param_type param(const OPERX& o)
		{
			return param_helper(o, typename make_indices<sizeof...(Ps)>::type{});
		}
		template<>
		typename D::param_type param<std::initializer_list<double>>(const OPERX& o)
		{
			// no constructor from iterator :-(
			return D::param_type(o.size(), 0, o.size(), [&o](double i) { return o[i]; });
		}
		// for distributions taking two vector constructors.
		template<>
		typename D::param_type param<std::vector<double>>(const OPERX& o)
		{
			// intervals in first half, densities in second
			return D::param_type(o.begin(), o.begin() + o.size()/2, o.begin() + o.size()/2);
		}

		T _op(engine::base_engine<>& g)
		{
			return D::operator()(g);
		}
		T _min() const
		{
			return D::min();
		}
		T _max() const
		{
			return D::max();
		}
	private:
		template<xword... Is>
		typename D::param_type param_helper(const OPERX& o, indices<Is...>)
		{
			return param_type(static_cast<Ps>(o[Is])...);
		}
	};

} // namespace distribution

