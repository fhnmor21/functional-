#ifndef FUNCTOR_UTILS_HPP
#define FUNCTOR_UTILS_HPP

#include "curry.hpp"
#include "functor.hpp"

namespace FunctionalCpp
{

  // fmap :: (a -> b) -> f a -> f b
  template < template < class... > class F, class Ret, class Arg >
  F< Ret > fmap(const Function1< Arg, Ret > a2b, const F< Arg >& fa)
  {
    return Functor< Function1< Arg, Ret >, F< Arg >, F< Ret > >::fmap(a2b, fa);
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
