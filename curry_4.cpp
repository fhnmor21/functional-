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

template<size_t level, size_t argsNum, typename Func, typename Args>
struct Closure
{
  static std::size_t constexpr index = (argsNum - level);

  using NextClosure = Closure<level-1, argsNum, Func, Args>;
  using An0 = typename std::tuple_element< index, Args>::type;
  // using An1 = typename std::tuple_element< index + 1, Args >::type;

  // CTOR
  Closure(Func& func, Args& args)
    : func_r(func)
    , args_r(args)
    , closure_m(new NextClosure(func_r, args_r))
  {
    ;
  }

  auto operator()(An0 an0)
  {
    std::get<index>(args_r) = an0;
    return *closure_m;
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

template<size_t argsNum, typename Func, typename Args>
struct Closure<1, argsNum, Func, Args>
{
  static std::size_t constexpr index = (argsNum - 1);

  using NextClosure = Closure<0, argsNum, Func, Args>;
  using An0 = typename std::tuple_element< index, Args>::type;
  // using An1 = typename std::tuple_element< index + 1, Args >::type;

  // CTOR
  Closure(Func& func, Args& args)
    : func_r(func)
    , args_r(args)
  {
    ;
  }

  auto operator()(An0 an0)
  {
    std::get<index>(args_r) = an0;
    return apply_from_tuple(func_r, args_r);
  }

  // Data
  Func& func_r;
  Args& args_r;
  std::unique_ptr< NextClosure > closure_m;
};

template < typename R, typename... As >
struct Curry
{
  static std::size_t constexpr argsNum = sizeof... (As);

  using Func = std::function<R(As...)>;
  using Args = std::tuple<As...>;
  using NextClosure = Closure<argsNum-1, argsNum, Func, Args>;
  using A0 = typename std::tuple_element< 0, Args >::type;
  // using A1 = typename std::tuple_element< 1, Args >::type;

  // CTOR
  Curry(Func func)
    : func_m(func)
    , closure_m(new NextClosure(func_m, args_m))
  {
    ;
  }

  // Methods
  auto operator()(A0 a0)
  {
    std::get<0>(args_m) = a0;
    return *closure_m;
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

template <size_t N, size_t M, typename An>
struct Onion
{
  template <typename Func, typename Args >
  static auto rec(Func func, Args args)
  {
    std::size_t constexpr index = (M-N);
    using A1 = typename std::tuple_element< index, Args>::type;

    return [&](An an)
    {
      std::get< index >(args) = an;
      return Onion< N-1 , M, A1 >::rec( func, args );
    };
  }
};


template <size_t M, typename An>
struct Onion<1, M, An>
{
  template <typename Func, typename Args >
  static auto rec(Func func, Args args )
  {
    return [&](An an)
    {
      std::get<M-1>(args) = an;
      return apply_from_tuple(func, args);
    };
  }
};


template< typename R, typename... As >
auto curry(R (*f) (As... as))
{
  using Func = std::function< R(As...) >;
  using Args = std::tuple< As... >;
  using A1 = typename std::tuple_element<0, Args>::type;
  std::size_t constexpr argsNum = sizeof... (As);

  Args args;
  Func func = f;

  return Onion< argsNum, argsNum, A1 >::rec( func, args );
}


int main()
{
    std::tuple<int, int> numbers2(1,2);
    std::cout <<  apply_from_tuple(add2, numbers2) << std::endl;
    std::tuple<int, int, int> numbers3(1,2,3);
    std::cout <<  apply_from_tuple(add3, numbers3) << std::endl;
    std::tuple<int, int, int, int> numbers4(1,2,3,4);
    std::cout <<  apply_from_tuple(add4, numbers4) << std::endl;

    auto f_ = curry(add3);
    std::cout << f_(33)(22)(11) << std::endl;
}
