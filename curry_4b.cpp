#include <cstddef>
#include <iostream>
#include <memory>
#include "apply_tuple.hpp"


// ===
template<size_t level, size_t argsNum, typename Func, typename ArgsPtr, typename R>
struct Closure
{
  static std::size_t constexpr index = (argsNum - level);
  using NextClosure = Closure<level-1, argsNum, Func, ArgsPtr, R>;
  using An0 = typename std::remove_pointer< typename std::tuple_element< index, ArgsPtr >::type >::type;

  // CTOR
  Closure(Func func, ArgsPtr& argsPtr)
    : func_r(func)
    , argsPtr_r(argsPtr)
  {
    closure_m.reset( new NextClosure(func_r, argsPtr_r) );
  }

  NextClosure& operator()(An0& an0)
  {
    std::get<index>(argsPtr_r) = &an0;
    return (*closure_m);
  }

  NextClosure& operator()(An0&& an0)
  {
    auto p = std::make_unique<An0>(std::move(an0));
    rArgs.swap(p);
    std::get<index>(argsPtr_r) = rArgs.get();
    return (*closure_m);
  }

  // Data
  Func func_r;
  ArgsPtr& argsPtr_r;
  std::unique_ptr< An0 > rArgs;
  std::unique_ptr< NextClosure > closure_m;
};

template<size_t argsNum, typename Func, typename ArgsPtr, typename R>
struct Closure<1, argsNum, Func, ArgsPtr, R>
{
  static std::size_t constexpr index = (argsNum - 1);
  using An0 = typename std::remove_pointer< typename std::tuple_element< index, ArgsPtr >::type >::type;

  Closure(Func func, ArgsPtr& argsPtr)
  : func_r(func)
  , argsPtr_r(argsPtr)
  {}

  R operator()(An0& an0)
  {
    std::get<index>(argsPtr_r) = &an0;
    return apply_from_ptr_tuple(func_r, argsPtr_r);
  }

  R operator()(An0&& an0)
  {
    auto p = std::make_unique<An0>(std::move(an0));
    rArgs.swap(p);
    std::get<index>(argsPtr_r) = rArgs.get();
    return apply_from_ptr_tuple(func_r, argsPtr_r);
  }

  // Data
  Func func_r;
  ArgsPtr& argsPtr_r;
  std::unique_ptr< An0 > rArgs;
};

template < typename R, typename... As >
struct Curry
{
  static std::size_t constexpr argsNum = sizeof... (As);

  using Func = std::function<R(As...)>;
  using ArgsPtr = std::tuple< typename std::add_pointer< typename std::remove_reference<As>::type>::type... >;

  using NextClosure = Closure<argsNum-1, argsNum, Func, ArgsPtr, R>;
  using A0 = typename std::remove_pointer< typename std::tuple_element< 0, ArgsPtr >::type >::type;

  // CTOR
  Curry(Func func)
  : func_m(func)
  {
    closure_m.reset( new NextClosure(func_m, argsPtr_m) );
  }

  // Methods
  NextClosure& operator()(const A0& a0)
  {
    std::get<0>(argsPtr_m) = &a0;
    return (*closure_m);
  }

  NextClosure& operator()(A0&& a0)
  {
    auto p = std::make_unique<A0>(std::move(a0));
    rArgs.swap(p);
    std::get<0>(argsPtr_m) = rArgs.get();
    return (*closure_m);
  }

  // Data
  Func func_m;
  ArgsPtr argsPtr_m;
  std::unique_ptr< A0 > rArgs;
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

int add6 (int a, float b, long c, double& d, bool cond, const std::string& e)
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

  auto f6 = curry(add6);
  std::cout << f6(4)(3)(2)(val)(false)(msg) << std::endl;
  auto f5 = curry(add5);
  std::cout << f5(4)(3)(2)(val)(true) << std::endl;
  auto f4 = curry(add4);
  std::cout << f4(4)(3)(2)(val) << std::endl;
  auto f3 = curry(add3);
  std::cout << f3(4)(3)(2) << std::endl;
  auto f2 = curry(add2);
  std::cout << f2(4)(3) << std::endl;

  // NOTE: this should not work because currying needs at least 2 arguments
  /* auto f1 = curry(add1);
     std::cout << f1(4) << std::endl;  */
}
