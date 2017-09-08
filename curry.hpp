#ifndef CURRY_HPP
#define CURRY_HPP

#include <memory>
#include "invoke.hpp"

namespace FunctionalCpp
{

  // Partial Function class used to hold partial applications
  // *******************************************************************
  template<size_t level, size_t argsNum, typename Func, typename ArgPtrs, typename ArgVals, typename R>
  struct PartialFn
  {
    static std::size_t constexpr index = (argsNum - level);
    using ReturnT = PartialFn<level-1, argsNum, Func, ArgPtrs, ArgVals, R>;
    using ArgT = typename std::remove_pointer< typename std::tuple_element< index, ArgPtrs >::type >::type;

    // CTOR
    PartialFn(Func func)
      : partialFn_p(new ReturnT(func))
      , argPtrs_p(partialFn_p->argPtrs_p)
      , argVals_p(partialFn_p->argVals_p)
    {}

    ReturnT operator()(ArgT& an)
    {
      std::get<index>(*argPtrs_p) = &an;
      return (*partialFn_p.release());
    }

    ReturnT operator()(ArgT&& an)
    {
      auto newVal = std::make_unique<ArgT>(std::move(an));
      auto val = std::get<index>(*argVals_p);
      val.swap(newVal);
      auto ptr = std::get<index>(*argPtrs_p);
      ptr = val.get();

      return (*partialFn_p.release());
    }

    // Data
    std::unique_ptr< ReturnT > partialFn_p;
    ArgPtrs* argPtrs_p;
    ArgVals* argVals_p;
  };

  // Partial Function specialization for the last argument
  // *******************************************************************
  template<size_t argsNum, typename Func, typename ArgPtrs, typename ArgVals, typename R>
  struct PartialFn<1, argsNum, Func, ArgPtrs, ArgVals, R>
  {
    static std::size_t constexpr index = (argsNum - 1);
    using ReturnT = R;
    using ArgT = typename std::remove_pointer< typename std::tuple_element< index, ArgPtrs >::type >::type;

    // CTOR
    PartialFn(Func func)
      : func_m(func)
      , argPtrs_m(new ArgPtrs)
      , argVals_m(new ArgVals)
      , argPtrs_p(argPtrs_m.get())
      , argVals_p(argVals_m.get())
    {}

    ReturnT operator()(ArgT& an)
    {
      std::get<index>(argPtrs_p) = &an;

      return Tuple::Ptrs::invoke(func_m, argPtrs_p);
    }

    ReturnT operator()(ArgT&& an)
    {
      auto newVal = std::make_unique<ArgT>(std::move(an));
      auto val = std::get<index>(*argVals_p);
      val.swap(newVal);
      auto ptr = std::get<index>(*argPtrs_p);
      ptr = val.get();

      return Tuple::Ptrs::invoke(func_m, argPtrs_p);
    }

    // Data
    Func func_m;
    std::unique_ptr< ArgPtrs > argPtrs_m;
    std::unique_ptr< ArgVals > argVals_m;
    ArgPtrs* argPtrs_p;
    ArgVals* argVals_p;
  };

    // Curry type holds the recursive pratial applications to a N-ary function
  // *******************************************************************
  template < typename R, typename... Args >
  struct Curry
  {
    static std::size_t constexpr argsNum = sizeof... (Args);

    using Func = std::function<R(Args...)>;
    using ArgPtrs = std::tuple< typename std::add_pointer< typename std::remove_reference<Args>::type>::type... >;
    using ArgVals = std::tuple< typename std::unique_ptr< typename std::remove_reference<Args>::type>... >;
    using ReturnT = PartialFn<argsNum, argsNum, Func, ArgPtrs, ArgVals, R>;

    static ReturnT&& Factory(Func func)
    {
      auto r = ReturnT(std::forward<Func>(func));
      return std::move(r);
    }

    // CTORS
    Curry() = delete;
    Curry(Curry&) = delete;
  private:
    Curry(Func func){}

  };

  // given a function will return a curry structure
  // needs a minimum of 2 arguments
  template< typename R, typename A1, typename A2, typename... Args >
  auto curry(R (*f) (A1, A2, Args... as))
  {
    return Curry<R, A1, A2, Args...>::Factory(std::function<R(A1, A2, Args...)>(f));
  }

} // end nasmespace FuncrtionalCpp

#endif
