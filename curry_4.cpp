#include <cstddef>
#include <iostream>
#include <memory>
#include "apply_tuple.hpp"


// test function
int add2 (int a, int b)
{
    return a+b;
}

int add3 (int a, int b, int c)
{
    return a+b+c;
}

int add4 (int a, float b, long c, double d)
{
    return a+b+c+d;
}


// ===

template<size_t level, size_t argsNum, typename Func, typename Args, typename R>
struct Closure
{
  static std::size_t constexpr index = (argsNum - level);

  using NextClosure = Closure<level-1, argsNum, Func, Args, R>;
  using An0 = typename std::tuple_element< index, Args>::type;
  // using An1 = typename std::tuple_element< index + 1, Args >::type;

  // CTOR
  Closure(Func& func, Args& args)
    : func_r(func)
    , args_r(args)
  {
    closure_m.reset( new NextClosure(func_r, args_r) );
  }

  NextClosure& operator()(An0 an0)
  {
    std::get<index>(args_r) = an0;
    return (*closure_m);
    // return [&](An1 an1)
    // {
    //   return closure_m->(an1);
    // };
  }

  // Data
  Func& func_r;
  Args& args_r;
  std::unique_ptr< NextClosure > closure_m;
};

template<size_t argsNum, typename Func, typename Args, typename R>
struct Closure<1, argsNum, Func, Args, R>
{
  static std::size_t constexpr index = (argsNum - 1);

  using An0 = typename std::tuple_element< index, Args>::type;
  // using An1 = typename std::tuple_element< index + 1, Args >::type;

  // CTOR
  Closure(Func& func, Args& args)
    : func_r(func)
    , args_r(args)
  {
    ;
  }

  R operator()(An0 an0)
  {
    std::get<index>(args_r) = an0;
    return apply_from_tuple(func_r, args_r);
  }

  // Data
  Func& func_r;
  Args& args_r;

};

template < typename R, typename... As >
struct Curry
{
  static std::size_t constexpr argsNum = sizeof... (As);

  using Func = std::function<R(As...)>;
  using Args = std::tuple<As...>;
  using NextClosure = Closure<argsNum-1, argsNum, Func, Args, R>;
  using A0 = typename std::tuple_element< 0, Args >::type;
  // using A1 = typename std::tuple_element< 1, Args >::type;

  // CTOR
  Curry(Func func)
    : func_m(func)
  {
    closure_m.reset( new NextClosure(func_m, args_m) );
  }

  // Methods
  NextClosure& operator()(A0 a0)
  {
    std::get<0>(args_m) = a0;
    return (*closure_m);
    // return [&](A1 a1)
    // {
      // return closure_m->(a1);
    // };
  }

  // Data
  Func func_m;
  Args args_m;
  std::unique_ptr< NextClosure > closure_m;

};

template< typename R, typename... As >
auto curry(R (*f) (As... as))
{
  return Curry< R, As... >( f);
}


int main()
{
    auto f4 = curry(add4);
    std::cout << f4(4)(3)(2)(1) << std::endl;
    auto f3 = curry(add3);
    std::cout << f3(4)(3)(2) << std::endl;
    auto f2 = curry(add2);
    std::cout << f2(4)(3) << std::endl;
}
