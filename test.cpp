#include <cstddef>
#include <iostream>

#include "arrow_utils.hpp"
#include "functor_utils.hpp"
#include "dict.hpp"
#include "list.hpp"
#include "vector.hpp"
#include "either.hpp"

using namespace FunctionalCpp;

// test function
int add1(double a) { return a + 1; }

int add2(int a, int b) { return a + b; }

int add3(int a, int b, int c) { return a + b + c; }

int add4(int a, float b, long c, double& d) { return a + b + c + d; }

long add5(int a, float b, long c, double& d, unsigned int& e) { return a + b + c + d + e; }

double add6(int a, float b, long c, double& d, unsigned int& e, const unsigned short f)
{
  return a + b + c + d + e + f;
}

double sqr(int a) { return a * a; }

auto rem = [](int a, int b) { return (a % b); };

float intTo2xFloat(int a) { return static_cast< float >(2 * a); }

bool isEqTo4f(float b) { return (b == 4.0f); }

std::string log(const std::string& fst) { return std::move("log: " + fst); }

// testing
int main()
{

  // TESTING: Partial application of arguments
  // ===
  auto args2 = Tuple< int, int >(4, 8);
  double val = 12.0;

  // ONE argument
  auto fp0 = curry(sqr);
  std::cerr << "\nfp0 wrapper call (5): " << (!fp0)(5) << std::endl;
  std::cerr << "fp0 wrapper partial_call (3): " << fp0(3) << std::endl;

  // TWO arguments
  auto fp1 = curry(add2);
  std::cerr << "\nfp1 wrapper call (!fp1)(5,6): " << (!fp1)(5, 6) << std::endl;
  std::cerr << "fp1 wrapper invoke (!fp1){4,8}: " << Vals::invoke((!fp1), args2) << std::endl;
  std::cerr << "fp1.fn partial call (~fp1)(6)(7): " << (~fp1)(6)(7) << std::endl;

  // TWO arguments + binding
  auto fp2 = curry(add2, 2);
  std::cerr << "\nfp2 wrapper call (!fp2)(5,6): " << (!fp2)(5, 6) << std::endl;
  std::cerr << "fp2 wrapper invoke (!fp2){4,8}: " << Vals::invoke((!fp2), args2) << std::endl;
  std::cerr << "fp2 wrapper partial_call fp2<2>|(3): " << fp2(3) << std::endl;
  std::cerr << "fp2.fn wrapper partial_call (~fp2)<2>|(3): " << (~fp2)(3) << std::endl;

  // ===
  auto args3 = Tuple< int, int, int >(2, 4, 8);

  // THREE arguments + binding
  auto fp3 = curry(add3, 2, 1);
  std::cerr << "\nfp3 wrapper call (!fp3)(5,6,7): " << (!fp3)(5, 6, 7) << std::endl;
  std::cerr << "fp3 wrapper invoke (!fp3){2,4,8}: " << Vals::invoke((!fp3), args3) << std::endl;
  std::cerr << "fp3 wrapper partial_call fp3<2, 1>|(3): " << fp3(3) << std::endl;
  std::cerr << "fp3.fn wrapper partial_call (~fp3)<2, 1>|(3): " << (~fp3)(3) << std::endl;

  auto fp4 = curry(add3, 1);
  std::cerr << "\nfp4 wrapper call (!fp4)(5,6,7): " << (!fp4)(5, 6, 7) << std::endl;
  std::cerr << "fp4 wrapper invoke (!fp4){2,4,8}: " << Vals::invoke((!fp4), args3) << std::endl;
  auto fp4_ = fp4(2);
  std::cerr << "fp4 wrapper partial_call fp4<1>|(2)|(3): " << fp4_(3) << std::endl;
  std::cerr << "fp4.fn wrapper partial_call (~fp4)<1>|(2)|(3): " << (~fp4_)(3) << std::endl;

  // ===
  auto args4 = Tuple< int, float, long, double >(12, 2.0, 4, 8.0);
  // FOUR arguments
  auto fp5 = curry(add4);
  std::cerr << "\nfp5 wrapper call (!fp5)(5,6.0,7,val=12.0): " << (!fp5)(5, 6.0f, 7l, val)
            << std::endl;
  std::cerr << "fp5 wrapper invoke (!fp5){12,2.0,4,8.0}: " << Vals::invoke((!fp5), args4)
            << std::endl;
  std::cerr << "fp5 wrapper partial_call fp5(5)(2.0)(7)(val=12): " << fp5(5)(2.0f)(7l)(val)
            << std::endl;
  std::cerr << "fp5.fn wrapper partial_call (~fp5)(5)(2.0)(7)(val=12): " << (~fp5)(5)(2.0f)(7l)(val)
            << std::endl;
  auto fp5_ = fp5(5);
  auto fp5__ = fp5_(2.0f);
  auto fp5___ = fp5__(7);
  std::cerr << "fp5 wrapper partial_call (5)|(2.0)|(7)|(val=12): " << fp5___(val) << std::endl;

  // ===
  unsigned int val_ = 5;
  auto args5 = Tuple< int, float, long, double, unsigned int >(12, 2.0, 4, 8.0, 9);
  // FIVE arguments + binding
  auto fp6 = curry(add5, 5);
  std::cerr << "\nfp6 wrapper call (!fp6)(5,6.0,7,val=12.0,val_=5): "
            << (!fp6)(5, 6.0f, 7, val, val_) << std::endl;
  std::cerr << "fp6 wrapper invoke (!fp6){12, 2.0, 4, 8.0, 9}: " << Vals::invoke((!fp6), args5)
            << std::endl;
  std::cerr << "fp6.fn wrapper partial_call (~fp6)<5>|(2.0)|(7)|(val=12.0)|(val_=5): "
            << (~fp6)(2.0f)(7l)(val)(val_) << std::endl;
  auto fp6__ = fp6(2.0f);
  auto fp6___ = fp6__(7);
  auto fp6_____ = fp6___(val);
  std::cout << "fp6  wrapper partial_call fp6<5>|(2.0)|(7)|(val=12.0)|(val_=5): " << fp6_____(val_)
            << std::endl;

  // ===
  auto args6 = Tuple< int, float, long, double, unsigned int, short >(12, 2.0f, 4l, 8.0, 9, 1);
  // SIX arguments + binding
  auto fp7 = curry(add6, 4);
  std::cerr << "\nfp7 wrapper call (!fp7)(4,6.0,7,val=12.0,val_=5,1): "
            << (!fp7)(4, 6.0f, 7, val, val_, 1) << std::endl;
  std::cerr << "fp7 wrapper invoke (!fp7){12, 2.0, 4, 8.0, 9, 1}: " << Vals::invoke((!fp7), args6)
            << std::endl;
  std::cerr << "fp7.fn wrapper partial_call (~fp7)<4>|(6.0)|(7)|(val=12.0)|(val_=5)|(1): "
            << (~fp7)(6.0f)(7l)(val)(val_)(1) << std::endl;
  auto fp7_ = fp7(6.0);
  auto fp7__ = fp7_(7);
  auto fp7___ = fp7__(val);
  auto fp7____ = fp7___(val_);
  std::cout << "fp7 wrapper partial_call <4>|(6.0)|(7)|(val=12.0)|(val_=5)|(1): " << fp7____(1)
            << std::endl;

  // ===
  // TWO arguments std::function
  Function2< int, int, int> sf2 = add2;
  auto sf2_ = curry(sf2);
  std::cerr << "\nsf2 add wrapper call (!sf2_)(7,4): " << (!sf2_)(7, 4) << std::endl;
  auto sf2__ = sf2_(8);
  std::cout << "sf2 add wrapper partial_call (8)|(5): " << sf2__(5) << std::endl;

  // ===
  // TWO arguments std::function wrapping a lambda
  Function2< int, int, int> sf3 = rem;
  auto sf3_ = curry(sf3);
  std::cerr << "\nsf3 mod wrapper call (!sf3_)(7,4): " << (!sf3_)(7, 4) << std::endl;
  auto sf3__ = sf3_(9);
  std::cout << "sf3 mod wrapper partial_call (9)|(5): " << sf3__(5) << std::endl;

  // ===
  // TWO arguments std::bind wrapping a function 3-ary (add3)
  // NOTE: this is redundant since we can bind values using curry() directly
  using namespace std::placeholders;
  Function2< int, int, int> sf4 = std::bind(add3, 5, _1, _2);
  auto sf4_ = curry(sf4);
  std::cerr << "\nsf4 add wrapper call (!sf4_)<5>|(7,4): " << (!sf4_)(7, 4) << std::endl;
  auto sf4__ = sf4_(8);
  std::cout << "sf4 add wrapper partial_call <5>|(8)|(7): " << sf4__(7) << std::endl;

  // ===
  // Category: Compose
  auto is2i = compose(isEqTo4f, intTo2xFloat);
  std::cout << "\nCompose: is2i = intTo2xFloat . isEqTo4f\n";
  std::string strIs2iOf2 = (is2i(2)) ? "True" : "False";
  std::cout << "is2i(2) : " << strIs2iOf2 << std::endl;
  ;
  std::string strIs2iOf3 = (is2i(3)) ? "True" : "False";
  std::cout << "is2i(3) : " << strIs2iOf3 << std::endl;
  ;

  // TODO: need to change named operator to be able to use templated functions
  // auto is2iOp = isEqTo4f <comp> intTo2xFloat;

  // ===
  // Arrow
  Tuple< int, int > num_args{10, 20};

  auto ppp_nums = prod3(~fp3, ~fp2)(num_args);
  std::cout << "\nArrow prod3 on {10, 20}\n";
  std::cout << "prod3(~fp3, ~fp2)({10, 20}): { " << std::get< 0 >(ppp_nums) << ", "
            << std::get< 1 >(ppp_nums) << " }\n";

  auto aaa_nums = and3(~fp3, ~fp2)(50);
  std::cout << "\nArrow and3 on 50\n";
  std::cout << "and3(~fp3, ~fp2)(50): { " << std::get< 0 >(aaa_nums) << ", "
            << std::get< 1 >(aaa_nums) << " }\n";

  auto first_fp3 = first(~fp3);
  auto fst_nums = first_fp3(num_args);
  std::cout << "\nArrow first from fp3\n";
  std::cout << "first(~fp3)({10, 20}): { " << std::get< 0 >(fst_nums) << ", "
            << std::get< 1 >(fst_nums) << " }\n";

  auto second_fp3 = second(~fp3);
  auto snd_nums = second_fp3(num_args);
  std::cout << "\nArrow first from fp3\n";
  std::cout << "second(~fp3)({10, 20}): { " << std::get< 0 >(snd_nums) << ", "
            << std::get< 1 >(snd_nums) << " }\n";

  auto log_ = curry(log);
  Tuple< int, std::string > old_cxt{10, "test"};
  auto new_cxt = prod3(~fp3, ~log_)(old_cxt);
  std::cout << "\nArrow prod3 on {10, \"test\"}\n";
  std::cout << "prod3(fp3, log)({10, \"test\"}): { " << std::get< 0 >(new_cxt) << ", \""
            << std::get< 1 >(new_cxt) << "\" }\n";

  // ===
  // FUNCTOR: Vector
  Vector< int > f_va_1{1, 2, 3, 4, 5};
  auto f_vb_1 = fmap(~fp2, f_va_1);

  std::cout << "\nFuntor fmap on Vector\n";
  std::cout << "fmap(~fp2<2>, f_va_1 = [1..5]) -> f_vb_1 = : ";
  for(auto& b : f_vb_1)
  {
    std::cout << b << " : ";
  }
  std::cout << std::endl;

  auto f_va2b = fmap(~fp1, f_va_1);
  auto f_vb_2 = apply(f_va2b, f_va_1);

  // ===
  // APPLICATTIVE: Vector
  size_t columns = 5;
  std::cout << "\nApplicative fmap & apply on Vector\n";
  std::cout << "fmap(~fp1, f_va_1 = [1..5]) -> f_va2b";
  std::cout << "\napply(fa2b, f_va_1 = [1..5]) -> f_vb_2 =\n : ";
  for(auto& b : f_vb_2)
  {
    std::cout << b;
    if(columns == 1)
    {
      std::cout << "\n : ";
      columns = 5;
    }
    else
    {
      std::cout << " : ";
      --columns;
    }
  }
  std::cout << std::endl;

  // ===
  // APPLICATTIVE: List
  List< int > f_la_1{11, 12, 13, 14, 15};
  auto f_la2b = fmap(~fp1, f_la_1);
  auto f_lb_2 = apply(f_la2b, f_la_1);

  columns = 5;
  std::cout << "\nApplicative fmap & apply on List\n";
  std::cout << "fmap(~fp1, f_la_1 = [1..5]) -> f_la_2";
  std::cout << "\napply(fa2b, f_la_1 = [1..5]) -> f_lb_2 =\n : ";

  for(auto& b : f_lb_2)
  {
    std::cout << b;
    if(columns == 1)
    {
      std::cout << "\n : ";
      columns = 5;
    }
    else
    {
      std::cout << " : ";
      --columns;
    }
  }
  std::cout << std::endl;


  // ===
  // BiFunctor: Dict
  Dict<std::string, int> f_dab = {{"one",21}, {"two",22}, {"three",23}, {"four",24}, {"five",25}};
  Function1<std::string, std::string> fst = [](const std::string& b) { return std::move(std::string("_" + b + "_")); };
  Function1<int, int> snd = [](int a){ return a+10; };
  auto f_dcb = bimap(fst, snd, f_dab);

  std::cout << std::endl;
  for(auto & item: f_dcb)
  {
    std::cout << item.first << "\t" << item.second << std::endl;
  }

  // ===
  // Either
  auto eL = Left<int, bool>(10);
  auto eR = Right<int, bool>(false);
  auto eL_val = eL();
  auto eR_val = eR();
  std::cout << "\nEither: Left = " << eL_val << " | Right = " << eR_val << std::endl;


  // ===
  // Other
  auto utL = ST::UnionType<int, bool>(10);
  auto utR = ST::UnionType<int, bool>(false);
  int valUtL1 = ST::UTGet<0>::value(utL);
  int valUtR1 = ST::UTGet<1>::value(utL);
  int valUtL2 = ST::UTGet<0>::value(utR);
  int valUtR2 = ST::UTGet<1>::value(utR);

  std::cout << "UnionType valUtL1: " << valUtL1 << std::endl;
  std::cout << "UnionType valUtR1: " << valUtR1 << std::endl;
  std::cout << "UnionType valUtL2: " << valUtL2 << std::endl;
  std::cout << "UnionType valUtR2: " << valUtR2 << std::endl;

  auto oL = Other<char, float>('a', nullptr);
  auto oR = Other<char, float>(nullptr, 10.5);

  // ===
  // Maybe
  // auto mbJ = Maybe<int>(10);
  // auto mbN = Maybe<int>();

}
