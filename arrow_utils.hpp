#ifndef ARROW_UTILS_HPP
#define ARROW_UTILS_HPP

#include "curry.hpp"
#include "arrow.hpp"

namespace FunctionalCpp
{
  // (***) :: (a `arr` c) -> (b `arr` d) -> ((a, b) `arr` (c, d))
  template < class A,
             class B,
             class C,
             class D >
  Function1< std::tuple<A, B>, std::tuple<C, D> > prod3( Function1<A, C> a2c, Function1<B, D> b2d )
  {
    return Arrow<A, B, C, D>::prod3( a2c, b2d );
  }

  template < class A,
             class B,
             class C,
             class D >
  auto prod3( C(a2c)(A), D(b2d)(B) )
  {
    Function1<A, C> a2c_ = a2c;
    Function1<B, D> b2d_ = b2d;
    return prod3( a2c_, b2d_ );
  }

  // (&&&) :: (a `arr` c) -> (a `arr` d) -> (a `arr` (c, d))
  template < class A,
             class C,
             class D >
  Function1< A, std::tuple<C, D> > and3( Function1<A, C> a2c, Function1<A, D> a2d )
  {
    Function1< A, std::tuple<C, D> > a2cd = [a2c, a2d] (A a)
    {
      std::tuple<A, A> aa{a, a};
      return Arrow<A, A, C, D>::prod3( a2c, a2d )(aa);
    };
    return std::move(a2cd);
  }

  template < class A,
             class C,
             class D >
  auto and3( C(a2c)(A), D(a2d)(A) )
  {
    Function1<A, C> a2c_ = a2c;
    Function1<A, D> a2d_ = a2d;
    return and3( a2c_, a2d_ );
  }

  // ===
  // class to encapsulate: first :: (a `arr` c) -> ((a, b) `arr` (c, b))
  // NOTE: needed to delay the deduction of the last argument <class B>
  template < class A,
             class C >
  struct First
  {
    First( Function1<A, C> a2c )
      : a2c_m(a2c) {}

    template < class B >
    std::tuple<C, B> operator()( std::tuple<A, B> ab)
    {
      std::tuple<C, B> cb = Arrow<A, B, C, B>::prod3( a2c_m, id<B> )(ab);
      return std::move(cb);
    }

  private:
    Function1<A, C> a2c_m;
  };

  // first :: (a `arr` c) -> ((a, b) `arr` (c, b))
  template < class A,
             class C >
  auto first( Function1<A, C> a2c )
  {
    return std::move( First<A, C>(a2c) );
  }

  template < class A,
             class C >
  auto first( C(a2c)(A) )
  {
    Function1<A, C> a2c_ = a2c;
    return first( a2c_ );
  }

  // ===
  // class to encapsulate: second :: (c `arr` d) -> ((b, c) `arr` (b, d))
  // NOTE: needed to delay the deduction of the last argument <class B>
  template < class C,
             class D >
  struct Second
  {
    Second( Function1<C, D> c2d )
    : c2d_m(c2d) {}

    template < class B >
    std::tuple<B, D> operator()( std::tuple<B, C> bc)
    {
      std::tuple<B, D> bd = Arrow<B, C, B, D>::prod3( id<B>, c2d_m )(bc);
      return std::move(bd);
    }

  private:
    Function1<C, D> c2d_m;
  };

  // second :: (c `arr` d) -> ((b, c) `arr` (b, d))
  template < class C,
             class D >
  auto second( Function1<C, D> c2d )
  {
    return std::move( Second<C,D>(c2d) );
  }

  template < class C,
             class D >
  auto second( D(c2d)(C) )
  {
    Function1<C, D> c2d_ = c2d;
    return second( c2d );
  }

} // end namespace FunctionalCpp

#endif // ARROW_UTILS_HPP
