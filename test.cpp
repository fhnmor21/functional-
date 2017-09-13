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

  // TESTING: function wrapper with Call and Invoke
  // ===
  auto args2 = std::tuple<int, int>(4,8);

  std::function<int(int,int)> std_add2 = add2;
  auto fw = fnWrapper(add2);
  auto std_fw = std::function<int(int,int)>(fw.fn);
  std::cerr << "add2 call (5,6): " << add2(5,6) << std::endl;
  std::cerr << "add2 invoke {4,8}: " << Tuple::Vals::invoke(add2, args2) << std::endl;
  std::cerr << "std_add2 call (5,6): " << std_add2(5,6) << std::endl;
  std::cerr << "std_add2 invoke {4,8}: " << Tuple::Vals::invoke(std_add2, args2) << std::endl;
  std::cerr << "fw call (5,6): " << fw(5,6) << std::endl;
  std::cerr << "fw invoke {4,8}: " << Tuple::Vals::invoke(fw.fn, args2) << std::endl;
  std::cerr << "std_fw invoke {4,8}: " << Tuple::Vals::invoke(std_fw, args2) << std::endl;

  // TESTING: Partial application of arguments
  // ===
  auto fp1 = curry(add2);
  std::cerr << "fp1 wrapper call (5,6): " << (fp1.func_m)(5,6) << std::endl;
  std::cerr << "fp1 wrapper invoke {4,8}: " << Tuple::Vals::invoke((fp1.func_m.fn), args2) << std::endl;

  auto fp2 = curry(add2, 2);
  std::cerr << "fp2 wrapper call (5,6): " << (fp2.func_m)(5,6)  << std::endl;
  std::cerr << "fp2 wrapper invoke {4,8}: " << Tuple::Vals::invoke((fp2.func_m.fn), args2) << std::endl;
  std::cerr << "fp2 wrapper partial_call (3): " << fp2(3) << std::endl;

  auto args3 = std::tuple<int, int, int>(2,4,8);

  auto fp3 = curry(add3, 2, 1);
  std::cerr << "fp3 wrapper call (5,6,7): " << (fp3.func_m)(5,6,7)  << std::endl;
  std::cerr << "fp3 wrapper invoke {2,4,8}: " << Tuple::Vals::invoke((fp3.func_m.fn), args3) << std::endl;
  std::cerr << "fp3 wrapper partial_call (3): " << fp3(3) << std::endl;

  auto fp4 = curry(add3, 1);
  std::cerr << "fp4 wrapper call (5,6,7): " << (fp4.func_m)(5,6,7)  << std::endl;
  std::cerr << "fp4 wrapper invoke {2,4,8}: " << Tuple::Vals::invoke((fp4.func_m.fn), args3) << std::endl;
  auto fp4_ = fp4(2);
  std::cerr << "fp4 wrapper partial_call (2)(3): " << fp4_(3) << std::endl;

  /*
  auto f_t3_ = f_t3(4);
  std::cerr << "f_t3 " << f_t3_(5) << std::endl;

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
