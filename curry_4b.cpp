#include <cstddef>
#include <iostream>
#include <memory>
#include "apply_tuple.hpp"


// ===
template<size_t level, size_t argsNum, typename Func, typename ArgsVal, typename ArgsPtr, typename R>
struct Closure
{
  static std::size_t constexpr index = (argsNum - level);
  using NextClosure = Closure<level-1, argsNum, Func, ArgsVal, ArgsPtr, R>;
  using An0 = typename std::tuple_element< index, ArgsVal>::type;

  // CTOR
  Closure(Func& func, ArgsVal& argsVal, ArgsPtr& argsPtr)
    : func_r(func)
    , argsVal_r(argsVal)
    , argsPtr_r(argsPtr)
    // , args_r(args)
  {
    closure_m.reset( new NextClosure(func_r, argsVal_r, argsPtr_r) );
  }

  NextClosure& operator()(const An0& an0)
  {
    std::get<index>(argsPtr_r) = &(an0);
    return (*closure_m);
  }

  NextClosure& operator()(An0&& an0)
  {
    std::get<index>(argsVal_r) = std::move(an0);
    std::get<index>(argsPtr_r) = &(std::get<index>(argsVal_r));
    return (*closure_m);
  }

  // Data
  Func& func_r;
  // Args& args_r;
  ArgsVal& argsVal_r;
  ArgsPtr& argsPtr_r;
  std::unique_ptr< NextClosure > closure_m;
};

template<size_t argsNum, typename Func, typename ArgsVal, typename ArgsPtr, typename R>
struct Closure<1, argsNum, Func, ArgsVal, ArgsPtr, R>
{
  static std::size_t constexpr index = (argsNum - 1);
  using An0 = typename std::tuple_element< index, ArgsVal>::type;

  Closure(Func& func, ArgsVal& argsVal, ArgsPtr& argsPtr)
  : func_r(func)
  , argsVal_r(argsVal)
  , argsPtr_r(argsPtr)
  // , args_r(args)
  {
    ;
  }

  R operator()(const An0& an0)
  {
    std::get<index>(argsPtr_r) = &(an0);
    return apply_from_tuple_ptr(func_r, argsPtr_r);
  }

  R operator()(An0&& an0)
  {
    std::get<index>(argsVal_r) = std::move(an0);
    std::get<index>(argsPtr_r) = &(std::get<index>(argsVal_r));
    return apply_from_tuple_ptr(func_r, argsPtr_r);
  }

  // Data
  Func& func_r;
  // Args& args_r;
  ArgsVal& argsVal_r;
  ArgsPtr& argsPtr_r;

};

template < typename R, typename... As >
struct Curry
{
  static std::size_t constexpr argsNum = sizeof... (As);

  using Func = std::function<R(As...)>;
  // using Args = std::tuple< typename std::remove_reference<As>::type... >;
  using ArgsVal = std::tuple< typename std::remove_reference<As>::type... >;
  using ArgsPtr = std::tuple< typename std::add_pointer<As>::type... >;
  using NextClosure = Closure<argsNum-1, argsNum, Func, ArgsVal, ArgsPtr, R>;
  using A0 = typename std::tuple_element< 0, ArgsVal >::type;

  // CTOR
  Curry(Func func)
  : func_m(func)
  {
    closure_m.reset( new NextClosure(func_m, argsVal_m, argsPtr_m) );
  }

  // Methods
  NextClosure& operator()(const A0& a0)
  {
    std::get<0>(argsPtr_m) = &a0;
    return (*closure_m);
  }

  NextClosure& operator()(A0&& a0)
  {
    std::get<0>(argsVal_m) = std::move(a0);
    std::get<0>(argsPtr_m) = &(std::get<0>(argsVal_m));
    return (*closure_m);
  }

  // Data
  Func & func_m;
  // Args args_m;
  ArgsVal argsVal_m;
  ArgsPtr argsPtr_m;
  std::unique_ptr< NextClosure > closure_m;

};

// needs a minimum of 2 arguments
template< typename R, typename A1, typename A2, typename... As >
auto curry(R (*f) (A1, A2, As... as))
{
  return Curry<R, A1, A2, As...>(std::function<R(A1&&, A2&&, As&&...)>(f));
}


// test function
int add1 (double a)
{
  return a+1;
}

int add2 (int a, int b)
{
  return a+b;
}

int add3 (int a, int b, int c)
{
  return a+b+c;
}

int add4 (int a, float b, long c, double& d)
{
  return a+b+c+d;
}

int add5 (int a, float b, long c, double d, bool cond)
{
  if(cond)
    {
      return a+b+c+d;
    }
  else
    {
      return 0;
    }
}

int add6 (int a, float b, long c, double& d, bool cond, std::string& e)
{
  if(!cond)
    {
      std::cerr << e << std::endl;
    }
  return a+b+c+d;
}

// testing
int main()
{
  double  val = 1;
  std::string msg("Hello Curry!\n");

  // auto f6 = curry(add6);
  // std::cout << f6(4)(3)(2)(val)(false)(msg) << std::endl;
  // auto f5 = curry(add5);
  // std::cout << f5(4)(3)(2)(val)(true) << std::endl;
  // auto f4 = curry(add4);
  // std::cout << f4(4)(3)(2)(val) << std::endl;
  // auto f3 = curry(add3);
  // std::cout << f3(4)(3)(2) << std::endl;
  auto f2 = curry(add2);
  std::cout << f2(4)(3) << std::endl;

  // NOTE: this should not work because currying needs at least 2 arguments
  /* auto f1 = curry(add1);
     std::cout << f1(4) << std::endl;  */
}
