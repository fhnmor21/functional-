#ifndef CURRY_HPP
#define CURRY_HPP

#include <memory>
#include "invoke.hpp"
#include <iostream>

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
        //, argPtrs_m(new ArgPtrs)
        //, argVals_m(new ArgVals)
        //, argPtrs_p(argPtrs_m.get())
        //, argVals_p(argVals_m.get())
    {
      argPtrs_m.reset(new ArgPtrs);
      argVals_m.reset(new ArgVals);
      argPtrs_p = argPtrs_m.get();
      argVals_p = argVals_m.get();
    }

    ReturnT operator()(ArgT& an)
    {
      std::get<index>(*argPtrs_m) = &an;

      return Tuple::Ptrs::invoke(func_m, *argPtrs_p);
    }

    ReturnT operator()(ArgT&& an)
    {
      auto newVal = std::make_unique<ArgT>(std::move(an));
      std::get<index>(*argVals_m).swap(newVal);
      std::get<index>(*argPtrs_m) = std::get<index>(*argVals_m).get();

      return Tuple::Ptrs::invoke(func_m, *argPtrs_p);
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




  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////
  template< size_t level, typename Func, typename ArgVals, typename R >
  struct PartialFunc
  {
    // Types
    static std::size_t constexpr argsNum = std::tuple_size<typename std::remove_reference<ArgVals>::type>::value;
    static std::size_t constexpr index = (argsNum - level);
    using ArgT = typename std::tuple_element< index, ArgVals >::type::element_type;
    // using ArgT = typename std::remove_pointer< typename std::tuple_element< index, ArgPtrs >::type >::type;
    using ReturnT = PartialFunc<level-1, Func, ArgVals, R>;

    // Ctor
    PartialFunc(Func func)
      : partialFunc_m(new ReturnT(func))
    {
      std::cerr << "PartialFunc CTOR > " << argsNum << " - " << level << " @ " << this << std::endl;
    }

    PartialFunc(PartialFunc&) = default;
    PartialFunc(PartialFunc&&) = default;


    ~PartialFunc()
    {
      std::cerr << "PartialFunc DTOR < " << argsNum << " - " << level << " @ " << this << std::endl;
    }

    //ReturnT operator()(ArgT& an)
    //{
    //  return partialFunc_m;
    //}

    auto operator()(ArgT&& an)
    {
      std::cerr << "Operator () > " << argsNum << " - " << level << " arg: " << an << " @ " << this << std::endl;
      return partialFunc_m->getFn();
    }

    void id()
    {
      std::cerr << "ID : " << this << std::endl;
    }

    std::function<ReturnT(ArgT)> getFn()
    {
      return std::function<ReturnT(ArgT)>(this);
    }

    // Data
    ReturnT* partialFunc_m;
  };

  template< typename Func, typename ArgVals, typename R >
  struct PartialFunc<1, Func, ArgVals, R>
  {
    // Types
    static std::size_t constexpr argsNum = std::tuple_size<typename std::remove_reference<ArgVals>::type>::value;
    static std::size_t constexpr index = (argsNum - 1);
    using ArgT = typename std::tuple_element< index, ArgVals >::type::element_type;
    using ReturnT = R;

    // Ctor
    PartialFunc(Func func)
      : func_m(func)
    {
      std::cerr << "PartialFunc CTOR > " << argsNum << " - 1 @ " << this << std::endl;
    }

    PartialFunc(PartialFunc&) = default;
    PartialFunc(PartialFunc&&) = default;


    ~PartialFunc()
    {
      std::cerr << "PartialFunc DTOR < " << argsNum << " - 1 @ " << this << std::endl;
    }

    //Func operator()(ArgT& an)
    //{
    //  return func_m;
    //}

    Func&& operator()(ArgT&& an)
    {
      std::cerr << "Operator () > " << argsNum << " - 1" << " arg: " << an << " @ " << this << std::endl;
      return std::move(func_m);
    }

    std::function<ReturnT(ArgT)> getFn()
    {
      return std::function<ReturnT(ArgT)>(this);
    }

    void id()
    {
      std::cerr << "ID : " << this << std::endl;
    }


    // Data
    Func func_m;
    //ArgVals argVals_m;
  };


  // given a function will return a curry structure
  // needs a minimum of 2 arguments
  // template< typename R, typename A1, typename A2, typename... Args >
  // auto curry(R (*f) (A1, A2, Args... as))
  template< typename R, typename... Args >
  auto curry(R (*f) (Args... as))
  {
    static std::size_t constexpr argsNum = sizeof... (Args);

    using Func = std::function<R(Args...)>;
    using ArgVals = std::tuple< typename std::unique_ptr< typename std::remove_reference<Args>::type>... >;
    using ReturnT = PartialFunc<argsNum, Func, ArgVals, R>;

    ReturnT* partialFunc = new ReturnT(std::function<R(Args...)>(f));
    partialFunc->id();

    return (*partialFunc);

    //return std::move(ReturnT(std::function<R(Args...)>(f)) );


    //return Curry<R, A1, A2, Args...>::Factory(std::function<R(A1, A2, Args...)>(f));
  }

} // end nasmespace FunctionalCpp

#endif
