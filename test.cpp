#include <cstddef>
#include <iostream>
#include "curry.hpp"

using namespace FunctionalCpp;

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

  auto f6 = make_curry(add6);
  std::cout << f6(4)(3)(2)(val)(false)(msg) << std::endl;
  auto f5 = make_curry(add5);
  std::cout << f5(4)(3)(2)(val)(true) << std::endl;
  auto f4 = make_curry(add4);
  std::cout << f4(4)(3)(2)(val) << std::endl;
  auto f3 = make_curry(add3);
  std::cout << f3(4)(3)(2) << std::endl;
  auto f2 = make_curry(add2);
  std::cout << f2(4)(3) << std::endl;

  // NOTE: this should not work because currying needs at least 2 arguments
  /* auto f1 = curry(add1);
     std::cout << f1(4) << std::endl;  */
}
