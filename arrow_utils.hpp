#ifndef ARROW_UTILS_HPP
#define ARROW_UTILS_HPP

#include "curry.hpp"
#include "arrow.hpp"

namespace FunctionalCpp
{
  template < class A,
             class B,
             class C >
  //first :: (a `arr` b) -> ((a, c) `arr` (b, c))
  Function< std::tuple<A, C>, std::tuple<B, C> >
  first( Function<A, B> a2b )
  {
    return Arrow<A, B, C>::first(a2b);
  }

  template < class B,
             class C,
             class D >
  // second :: (c `arr` d) -> ((b, c) `arr` (b, d))
  Function< std::tuple<B, C>, std::tuple<B, D> >
  second( Function<C, D> c2d )
  {
    Function< std::tuple<B, C>, std::tuple<B, D> > snd = [c2d](std::tuple<B, C> bc)
    {
      return Tuple::swap(first(c2d)(Tuple::swap(bc)));
    };
    return snd;
  }

  // (***) :: (a `arr` c) -> (b `arr` d) -> ((a, b) `arr` (c, d))
  template < class A,
             class B,
             class C,
             class D >
  Function< std::tuple<A, B>, std::tuple<C, D> > prod3( Function<A, C> a2c, Function<B, D> b2d )
  {
    auto do_fst = make_curried(first<A,B,C>,  a2c);
    auto do_snd = make_curried(second<B,C,D>, b2d);
    return compose(~do_fst, ~do_snd);
  }

  // (&&&) :: (b `arr` c) -> (b `arr` c') -> (b `arr` (c, c'))


} // end namespace FunctionalCpp

#endif // ARROW_UTILS_HPP
