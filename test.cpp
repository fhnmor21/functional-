#include <cstddef>
#include <iostream>
#include "curry.hpp"
#include "functor.hpp"

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
  auto f = [](int a){return a;};
  auto f_t = Curry<int, int>::Factory(std::function<int(int)>(f));
  std::cout << f_t(4) << std::endl;

  // double  val = 1;
  // std::string msg("Hello Curry!\n");

  // auto f6 = curry(add6);
  // std::cout << f6(4)(3)(2)(val)(false)(msg) << std::endl;

  // auto f5 = curry(add5);
  // std::cout << f5(4)(3)(2)(val)(true) << std::endl;

  // auto f4 = curry(add4);
  // std::cout << f4(4)(3)(2)(val) << std::endl;

  // auto f3 = curry(add3);
  // std::cout << f3(4)(3)(2) << std::endl;

  // int val_3 = 3;
  // auto f2 = curry(add2);
  // std::cout << f2(val_3)(4) << std::endl;

  // auto plus3 = f2(3);
  // std::vector<int> input{0,1,2,3,4,5,6,7,8,9};
  // auto output = fmap(plus3, input);
  // for(auto& o: output)
  // {
  //   std::cout << 
  // }

  // NOTE: this should not work because currying needs at least 2 arguments
  /* auto f1 = curry(add1);
     std::cout << f1(4) << std::endl;  */
}
