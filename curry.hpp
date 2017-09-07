#include <memory>
#include "invoke.hpp"

#ifndef CURRY_HPP
#define CURRY_HPP

namespace FunctionalCpp
{

  // Partial Function class used to hold partial applications
  // *******************************************************************
  template<size_t level, size_t argsNum, typename Func, typename ArgsPtr, typename R>
  struct PartialFn
  {
    static std::size_t constexpr index = (argsNum - level);
    using ReturnT = PartialFn<level-1, argsNum, Func, ArgsPtr, R>;
    using ArgT = typename std::remove_pointer< typename std::tuple_element< index, ArgsPtr >::type >::type;

    // CTOR
    PartialFn(Func func, ArgsPtr& argsPtr)
      : func_r(func)
      , argsPtr_r(argsPtr)
    {
      partialFn_m.reset( new ReturnT(func_r, argsPtr_r) );
    }

    ReturnT& operator()(ArgT& an)
    {
      std::get<index>(argsPtr_r) = &an;
      return (*partialFn_m);
    }

    ReturnT& operator()(ArgT&& an)
    {
      auto p = std::make_unique<ArgT>(std::move(an));
      rArgs.swap(p);
      std::get<index>(argsPtr_r) = rArgs.get();
      return (*partialFn_m);
    }

    // Data
    Func func_r;
    ArgsPtr& argsPtr_r;
    std::unique_ptr< ArgT > rArgs;
    std::unique_ptr< ReturnT > partialFn_m;
  };

  // Partial Function specialization for the last argument
  // *******************************************************************
  template<size_t argsNum, typename Func, typename ArgsPtr, typename R>
  struct PartialFn<1, argsNum, Func, ArgsPtr, R>
  {
    static std::size_t constexpr index = (argsNum - 1);
    using ReturnT = R;
    using ArgT = typename std::remove_pointer< typename std::tuple_element< index, ArgsPtr >::type >::type;

    PartialFn(Func func, ArgsPtr& argsPtr)
    : func_r(func)
    , argsPtr_r(argsPtr)
    {}

    ReturnT operator()(ArgT& an)
    {
      std::get<index>(argsPtr_r) = &an;
      return Tuple::Ptrs::invoke(func_r, argsPtr_r);
    }

    ReturnT operator()(ArgT&& an)
    {
      auto p = std::make_unique<ArgT>(std::move(an));
      rArgs.swap(p);
      std::get<index>(argsPtr_r) = rArgs.get();
      return Tuple::Ptrs::invoke(func_r, argsPtr_r);
    }

    // Data
    Func func_r;
    ArgsPtr& argsPtr_r;
    std::unique_ptr< ArgT > rArgs;
  };


  // Curry type holds the recursive pratial applications to a N-ary function
  // *******************************************************************
  template < typename R, typename... Args >
  struct Curry
  {
    static std::size_t constexpr argsNum = sizeof... (Args);

    using Func = std::function<R(Args...)>;
    using ArgsPtr = std::tuple< typename std::add_pointer< typename std::remove_reference<Args>::type>::type... >;

    using ReturnT = PartialFn<argsNum-1, argsNum, Func, ArgsPtr, R>;
    using ArgN = typename std::remove_pointer< typename std::tuple_element< 0, ArgsPtr >::type >::type;

    // CTOR
    Curry(Func func)
    : func_m(func)
    {
      partialFn_m.reset( new ReturnT(func_m, argsPtr_m) );
    }

    // Methods
    ReturnT& operator()(const ArgN& a0)
    {
      std::get<0>(argsPtr_m) = &a0;
      return (*partialFn_m);
    }

    ReturnT& operator()(ArgN&& a0)
    {
      auto p = std::make_unique<ArgN>(std::move(a0));
      rArgs.swap(p);
      std::get<0>(argsPtr_m) = rArgs.get();
      return (*partialFn_m);
    }

    // Data
    Func func_m;
    ArgsPtr argsPtr_m;
    std::unique_ptr< ArgN > rArgs;
    std::unique_ptr< ReturnT > partialFn_m;

  };

  // given a function will return a curry structure
  // needs a minimum of 2 arguments
  template< typename R, typename A1, typename A2, typename... Args >
  auto make_curry(R (*f) (A1, A2, Args... as))
  {
    return Curry<R, A1, A2, Args...>(std::function<R(A1&&, A2&&, Args&&...)>(f));
  }

} // end nasmespace FuncrtionalCpp

#endif
