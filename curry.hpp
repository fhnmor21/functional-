#include <memory>
#include "apply_tuple.hpp"

#ifndef CURRY_HPP
#define CURRY_HPP

namespace FunctionalCpp
{

  // Clousure class used to hold partial applications
  // *******************************************************************
  template<size_t level, size_t argsNum, typename Func, typename ArgsPtr, typename R>
  struct Closure
  {
    static std::size_t constexpr index = (argsNum - level);
    using ReturnT = Closure<level-1, argsNum, Func, ArgsPtr, R>;
    using ArgN = typename std::remove_pointer< typename std::tuple_element< index, ArgsPtr >::type >::type;

    // CTOR
    Closure(Func func, ArgsPtr& argsPtr)
      : func_r(func)
      , argsPtr_r(argsPtr)
    {
      closure_m.reset( new ReturnT(func_r, argsPtr_r) );
    }

    ReturnT& operator()(ArgN& an)
    {
      std::get<index>(argsPtr_r) = &an;
      return (*closure_m);
    }

    ReturnT& operator()(ArgN&& an)
    {
      auto p = std::make_unique<ArgN>(std::move(an));
      rArgs.swap(p);
      std::get<index>(argsPtr_r) = rArgs.get();
      return (*closure_m);
    }

    // Data
    Func func_r;
    ArgsPtr& argsPtr_r;
    std::unique_ptr< ArgN > rArgs;
    std::unique_ptr< ReturnT > closure_m;
  };

  // parttila specialization for the last argument
  // *******************************************************************
  template<size_t argsNum, typename Func, typename ArgsPtr, typename R>
  struct Closure<1, argsNum, Func, ArgsPtr, R>
  {
    static std::size_t constexpr index = (argsNum - 1);
    using ReturnT = R;
    using ArgN = typename std::remove_pointer< typename std::tuple_element< index, ArgsPtr >::type >::type;

    Closure(Func func, ArgsPtr& argsPtr)
    : func_r(func)
    , argsPtr_r(argsPtr)
    {}

    ReturnT operator()(ArgN& an)
    {
      std::get<index>(argsPtr_r) = &an;
      return Tuple::apply_ptr(func_r, argsPtr_r);
    }

    ReturnT operator()(ArgN&& an)
    {
      auto p = std::make_unique<ArgN>(std::move(an));
      rArgs.swap(p);
      std::get<index>(argsPtr_r) = rArgs.get();
      return Tuple::apply_ptr(func_r, argsPtr_r);
    }

    // Data
    Func func_r;
    ArgsPtr& argsPtr_r;
    std::unique_ptr< ArgN > rArgs;
  };


  // Curry type holds the recursive pratial applications to a N-ary function
  // *******************************************************************
  template < typename R, typename... As >
  struct Curry
  {
    static std::size_t constexpr argsNum = sizeof... (As);

    using Func = std::function<R(As...)>;
    using ArgsPtr = std::tuple< typename std::add_pointer< typename std::remove_reference<As>::type>::type... >;

    using ReturnT = Closure<argsNum-1, argsNum, Func, ArgsPtr, R>;
    using ArgN = typename std::remove_pointer< typename std::tuple_element< 0, ArgsPtr >::type >::type;

    // CTOR
    Curry(Func func)
    : func_m(func)
    {
      closure_m.reset( new ReturnT(func_m, argsPtr_m) );
    }

    // Methods
    ReturnT& operator()(const ArgN& a0)
    {
      std::get<0>(argsPtr_m) = &a0;
      return (*closure_m);
    }

    ReturnT& operator()(ArgN&& a0)
    {
      auto p = std::make_unique<ArgN>(std::move(a0));
      rArgs.swap(p);
      std::get<0>(argsPtr_m) = rArgs.get();
      return (*closure_m);
    }

    // Data
    Func func_m;
    ArgsPtr argsPtr_m;
    std::unique_ptr< ArgN > rArgs;
    std::unique_ptr< ReturnT > closure_m;

  };

  // given a function will return a curry structure
  // needs a minimum of 2 arguments
  template< typename R, typename A1, typename A2, typename... As >
  auto make_curry(R (*f) (A1, A2, As... as))
  {
    return Curry<R, A1, A2, As...>(std::function<R(A1&&, A2&&, As&&...)>(f));
  }

} // end nasmespace FuncrtionalCpp

#endif
