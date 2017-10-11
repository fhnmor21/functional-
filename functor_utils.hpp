#ifndef FUNCTOR_UTILS_HPP
#define FUNCTOR_UTILS_HPP

#include "curry.hpp"
#include "functor.hpp"
#include "bifunctor.hpp"
#include "applicative.hpp"

namespace FunctionalCpp
{

  // fmap :: (a -> b) -> f a -> f b
  template < template < class... > class F, class Ret, class Arg >
  F< Ret > fmap(const Function1< Arg, Ret > a2b, const F< Arg >& fa)
  {
    return Functor< Function1< Arg, Ret >, F< Arg >, F< Ret > >::fmap(a2b, fa);
  }

  // bimap :: (a -> c) -> (b -> d) f a b -> f b c
  template < template < class... > class F, class Ret1, class Arg1, class Ret2, class Arg2 >
  F< Ret1, Ret2 > bimap(const Function1< Arg1, Ret1 > a2c, const Function1< Arg2, Ret2 > b2d, const F< Arg1, Arg2 >& fab)
  {
    //    template < class ArrowA2C, class ArrowB2D, class FunctorOf_AB, class FunctorOf_CD >
    //    static FunctorOf_CD bimap(const ArrowA2C a2c, const ArrowB2D b2d, const FunctorOf_AB& fab);
    return BiFunctor< Function1< Arg1, Ret1 >, Function1< Arg2, Ret2 >, F< Arg1, Arg2 >, F< Ret1, Ret2 > >::bimap(a2c, b2d, fab);
  }

  // apply :: f (a -> b) -> f a -> f b
  template < template < class... > class F, class Ret, class Arg >
  F< Ret > apply(const F< Function1< Arg, Ret > >& fa2b, const F< Arg >& fa)
  {
    return Applicative< F< Function1< Arg, Ret > >, F< Arg >, F< Ret > >::apply(fa2b, fa);
  }

  // when :: Applicative f => Bool -> f () -> f ()

  // unless :: Applicative f => Bool -> f () -> f ()

} // end namespace FunctionalCpp

#endif // FUNCTOR_UTILS_HPP
