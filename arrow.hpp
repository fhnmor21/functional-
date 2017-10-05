#ifndef ARROW_HPP
#define ARROW_HPP

#include "function.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{

  // ===
  // Category static class
  template < class A, class B, class C >
  struct Cat
  {
    // (.) :: (b `arr` c) -> (a `arr` b) -> (a `arr` c)
    static Function1< A, C > compose(Function1< B, C > b2c, Function1< A, B > a2b)
    {
      Function1< A, C > a2c = [b2c, a2b](A a) { return b2c(a2b(a)); };
      return std::move(a2c);
    }
  };

  template < class A, class B, class C >
  // (.) :: (b `arr` c) -> (a `arr` b) -> (a `arr` c)
  Function1< A, C > compose(Function1< B, C > b2c, Function1< A, B > a2b)
  {
    return Cat< A, B, C >::compose(a2b, b2c);
  }

  template < class A, class B, class C >
  // (.) :: (b `arr` c) -> (a `arr` b) -> (a `arr` c)
  Function1< A, C > compose(C(b2c)(B), B(a2b)(A))
  {
    Function1< A, B > a2b_ = a2b;
    Function1< B, C > b2c_ = b2c;
    Function1< A, C > a2c_ = Cat< A, B, C >::compose(b2c_, a2b_);
    return a2c_;
  }

  // ===
  // Arrow static class
  template < class A, class B, class C, class D >
  struct Arrow : Cat< A, B, C >
  {
    // (***) :: (a `arr` c) -> (b `arr` d) -> ((a, b) `arr` (c, d))
    static Function1< Tuple< A, B >, Tuple< C, D > > prod3(Function1< A, C > a2c,
                                                           Function1< B, D > b2d)
    {
      Function1< Tuple< A, B >, Tuple< C, D > > ppp = [a2c, b2d](Tuple< A, B > ab) {
        return std::move(std::make_tuple< C, D >(a2c(std::get< 0 >(ab)), b2d(std::get< 1 >(ab))));
      };
      return ppp;
    }
  };

} // end namespace FunctionalCpp

#endif // ARROW_HPP
