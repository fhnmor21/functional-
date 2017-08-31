#include <cstddef>
#include <iostream>
#include "apply_tuple.hpp"

template <size_t N, size_t M, typename An>
struct Onion
{
  template <typename Func, typename Args >
  static auto rec(Func func, Args args)
  {
    std::size_t constexpr index = M - N;
    using A1 = typename std::tuple_element< index, Args>::type;
    
    return [&](An an)
    {
      std::get<index >(args) = an;
      return Onion< N-1 , M, A1 >::rec( func, args );
    };
  }
};

template <size_t M, typename An>
struct Onion<0, M, An>
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

  return Onion< argsNum-1, argsNum, A1 >::rec( func, args );
}

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


int main()
{
    std::tuple<int, int> numbers2(1,2);
    std::cout <<  apply_from_tuple(add2, numbers2) << std::endl;
    std::tuple<int, int, int> numbers3(1,2,3);
    std::cout <<  apply_from_tuple(add3, numbers3) << std::endl;
    std::tuple<int, int, int, int> numbers4(1,2,3,4);
    std::cout <<  apply_from_tuple(add4, numbers4) << std::endl;

    auto f4 = curry(add4);
    std::cout << f4(40)(30)(20)(10) << std::endl;
}
