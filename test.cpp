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
  auto args = std::tuple<int, int>(4,8);

  std::function<int(int,int)> std_add2 = add2;
  auto fw = fnWrapper(add2);
  auto std_fw = std::function<int(int,int)>(fw.fn);
  std::cerr << "add2 call (5,6): " << add2(5,6) << std::endl;
  std::cerr << "add2 invoke {4,8}: " << Tuple::Vals::invoke(add2, args) << std::endl;
  std::cerr << "std_add2 call (5,6): " << std_add2(5,6) << std::endl;
  std::cerr << "std_add2 invoke {4,8}: " << Tuple::Vals::invoke(std_add2, args) << std::endl;
  std::cerr << "fw call (5,6): " << fw(5,6) << std::endl;
  std::cerr << "fw invoke {4,8}: " << Tuple::Vals::invoke(fw.fn, args) << std::endl;
  std::cerr << "std_fw invoke {4,8}: " << Tuple::Vals::invoke(std_fw, args) << std::endl;
  


  //auto fp1 = partial(add2, 3);
  //auto r = fp1.func_m.fn(2,2);
  //std::cerr << r << " : fp1 " << (fp1.func_m)(4,3) << std::endl;
  //std::cerr << "fp1_ " << fp1(3) << std::endl;

  //auto fp2 = partial(add2);
  //std::cerr << "fp2 " << (fp2.func_m)(4,5) << std::endl;


  // auto f_t2 = curry(add2);
  // auto f_t2_ = f_t2(4);
  // std::cerr << "f_t2 " << f_t2_(5) << std::endl;

  /*
  auto f_t3 = curry(add3);
  auto f_t3_ = f_t3(4);
  std::cerr << "f_t3 " << f_t3_(5)(6) << std::endl;

  double  val = 10;
  auto f_t4 = curry(add4);
  auto f_t4_ = f_t4(4);
  std::cerr << "f_t4 " << f_t4_(5)(6)(val) << std::endl;

  std::string msg("Hello Curry!\n");

  auto f6 = curry(add6);
  std::cout << "f6 " << f6(4)(3)(2)(val)(false)(msg) << std::endl;

  auto f5 = curry(add5);
  std::cout << "f5 " << f5(4)(3)(2)(val)(true) << std::endl;

  auto f4 = curry(add4);
  std::cout << "f4 " << f4(4)(3)(2)(val) << std::endl;

  auto f3 = curry(add3);
  std::cout << "f3 " << f3(4)(3)(2) << std::endl;

  int val_3 = 3;
  auto f2 = curry(add2);
  std::cout << "f2 " << f2(val_3)(4) << std::endl;
*/
  // auto plus3 = f2(3);
  // std::vector<int> input{0,1,2,3,4,5,6,7,8,9};
  // auto output = fmap<>(plus3, input);
  // for(auto& o: output)
  // {
  //   std::cout << "Functor" ;
  // }

  // NOTE: this should not work because currying needs at least 2 arguments
  /* auto f1 = curry(add1);
     std::cout << f1(4) << std::endl;  */
}
