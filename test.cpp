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

long add5 (int a, float b, long c, double& d, unsigned int& e)
{
  return a+b+c+d+e;
}

double add6 (int a, float b, long c, double& d, unsigned int& e, const unsigned short f)
{
  return a+b+c+d+e+f;
}



// testing 
int main()
{

  // TESTING: function wrapper with Call and Invoke
  // ===
  auto args2 = std::tuple<int, int>(4, 8);
  double  val = 12.0;

  std::function<int(int,int)> std_add2 = add2;
  auto fw = fnWrapper(add2);
  auto std_fw = std::function<int(int,int)>(~fw);
  std::cerr << "add2 call (5,6): " << add2(5,6) << std::endl;
  std::cerr << "add2 invoke {4,8}: " << Tuple::Vals::invoke(add2, args2) << std::endl;
  std::cerr << "std_add2 call (5,6): " << std_add2(5,6) << std::endl;
  std::cerr << "std_add2 invoke {4,8}: " << Tuple::Vals::invoke(std_add2, args2) << std::endl;
  std::cerr << "fw call (5,6): " << fw(5,6) << std::endl;
  std::cerr << "fw invoke {4,8}: " << Tuple::Vals::invoke(~fw, args2) << std::endl;
  std::cerr << "std_fw invoke {4,8}: " << Tuple::Vals::invoke(std_fw, args2) << std::endl;

  // TESTING: Partial application of arguments
  // ===

  // TWO arguments
  auto fp1 = curry(add2);
  std::cerr << "\nfp1 wrapper call (5,6): " << (~fp1)(5,6) << std::endl;
  std::cerr << "fp1 wrapper invoke {4,8}: " << Tuple::Vals::invoke((~~fp1), args2) << std::endl;

  // TWO arguments + binding
  auto fp2 = curry(add2, 2);
  std::cerr << "\nfp2 wrapper call (5,6): " << (~fp2)(5,6)  << std::endl;
  std::cerr << "fp2 wrapper invoke {4,8}: " << Tuple::Vals::invoke((~~fp2), args2) << std::endl;
  std::cerr << "fp2 wrapper partial_call <2>|(3): " << fp2(3) << std::endl;

  // ===
  auto args3 = std::tuple<int, int, int>(2, 4, 8);
  // THREE arguments + binding
  auto fp3 = curry(add3, 2, 1);
  std::cerr << "\nfp3 wrapper call (5,6,7): " << (~fp3)(5,6,7)  << std::endl;
  std::cerr << "fp3 wrapper invoke {2,4,8}: " << Tuple::Vals::invoke((~~fp3), args3) << std::endl;
  std::cerr << "fp3 wrapper partial_call <2, 1>|(3): " << fp3(3) << std::endl;

  auto fp4 = curry(add3, 1);
  std::cerr << "\nfp4 wrapper call (5,6,7): " << (~fp4)(5,6,7)  << std::endl;
  std::cerr << "fp4 wrapper invoke {2,4,8}: " << Tuple::Vals::invoke((~~fp4), args3) << std::endl;
  auto fp4_ = fp4(2);
  std::cerr << "fp4 wrapper partial_call <1>|(2)|(3): " << fp4_(3) << std::endl;

  // ===
  auto args4 = std::tuple<int, float, long, double>(12, 2.0, 4, 8.0);
  // FOUR arguments
  auto fp5 = curry(add4);
  std::cerr << "\nfp5 wrapper call (5,6.0,7,val=12.0): " << (~fp5)(5,6.0f,7l,val)  << std::endl;
  std::cerr << "fp5 wrapper invoke {12,2.0,4,8.0}: " << Tuple::Vals::invoke((~~fp5), args4) << std::endl;
  std::cerr << "fp5 wrapper partial_call (5)(2.0)(7)(val=12): " << fp5(5)(2.0f)(7l)(val) << std::endl;
  auto fp5_ = fp5(5);
  auto fp5__ = fp5_(2.0f);
  auto fp5___ = fp5__(7);
  std::cerr << "fp5 wrapper partial_call (5)|(2.0)|(7)|(val=12): " << fp5___(val) << std::endl;

  // ===
  unsigned int val_ = 5;
  auto args5 = std::tuple<int, float, long, double, unsigned int>(12, 2.0, 4, 8.0, 9);
  // FIVE arguments + binding
  auto fp6 = curry(add5, 5);
  std::cerr << "\nfp6 wrapper call (5,6.0,7,val=12.0,val_=5): " << (~fp6)(5,6.0f,7,val,val_) << std::endl;
  std::cerr << "fp6 wrapper invoke {12, 2.0, 4, 8.0, 9}: " << Tuple::Vals::invoke((~~fp6), args5) << std::endl;
  auto fp6__ = fp6(2.0f);
  auto fp6___ = fp6__(7);
  auto fp6_____ = fp6___(val);
  std::cout << "fp6  wrapper partial_call <5>|(2.0)|(7)|(val=12.0)|(val_=5): " << fp6_____(val_) << std::endl;

  // ===
  auto args6 = std::tuple<int, float, long, double, unsigned int, short>(12, 2.0f, 4l, 8.0, 9, 1);
  // SIX arguments + binding
  auto fp7 = curry(add6, 4);
  std::cerr << "\nfp7 wrapper call (4,6.0,7,val=12.0,val_=5,1): " << (~fp7)(4,6.0f,7,val,val_,1) << std::endl;
  std::cerr << "fp7 wrapper invoke {12, 2.0, 4, 8.0, 9, 1}: " << Tuple::Vals::invoke((~~fp7), args6) << std::endl;
  auto fp7_ = fp7(6.0);
  auto fp7__ = fp7_(7);
  auto fp7___ = fp7__(val);
  auto fp7____ = fp7___(val_);
  std::cout << "fp7  wrapper partial_call <4>|(6.0)|(7)|(val=12.0)|(val_=5)|(1): " << fp7____(1) << std::endl;

  // NOTE: this should not work because currying needs at least 2 arguments
  /* auto f1 = curry(add1);
     std::cout << f1(4) << std::endl;  */
}
