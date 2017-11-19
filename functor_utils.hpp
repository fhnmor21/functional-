#ifndef FUNCTOR_UTILS_HPP
#define FUNCTOR_UTILS_HPP

#include "curry.hpp"
#include "functor.hpp"
#include "bifunctor.hpp"
#include "contrafunctor.hpp"
#include "profunctor.hpp"
#include "applicative.hpp"

namespace FunctionalCpp
{

  // fmap :: (a -> b) -> f a -> f b
  template < template < class... > class F, class Ret, class Arg >
  F< Ret > fmap(const Function1< Arg, Ret > a2b, const F< Arg >& fa)
  {
    return Functor< Function1< Arg, Ret >, F< Arg >, F< Ret > >::fmap(a2b, fa);
  }

  // contramap :: (b -> a) -> f a -> f b
  template < template < class... > class F, class Ret, class Arg >
  F< Ret > contramap(const Function1< Ret, Arg > a2b, const F< Arg >& fa)
  {
    return ContraFunctor< Function1< Ret, Arg >, F< Arg >, F< Ret > >::contramap(a2b, fa);
  }

  // bimap :: (a -> c) -> (b -> d) -> f a b -> f c d
  template < template < class... > class F, class Ret1, class Arg1, class Ret2, class Arg2 >
  F< Ret1, Ret2 > bimap(const Function1< Arg1, Ret1 > a2c, const Function1< Arg2, Ret2 > b2d, const F< Arg1, Arg2 >& fab)
  {
    return BiFunctor< Function1< Arg1, Ret1 >, Function1< Arg2, Ret2 >, F< Arg1, Arg2 >, F< Ret1, Ret2 > >::bimap(a2c, b2d, fab);
  }

  // first :: (a -> c) -> f a b -> f c b
  template < template < class... > class F, class Ret1, class Arg1, class Arg2 >
  F< Ret1, Arg2 > first(const Function1< Arg1, Ret1 > a2c, const F< Arg1, Arg2 >& fab)
  {
    const Function1<Arg2, Arg2> b2d = id<Arg2>;
    return BiFunctor< Function1< Arg1, Ret1 >, Function1< Arg2, Arg2 >, F< Arg1, Arg2 >, F< Ret1, Arg2 > >::bimap(a2c, b2d, fab);
  }

  // second :: (b -> d) -> f a b -> f a d
  template < template < class... > class F, class Ret2, class Arg1, class Arg2 >
  F< Arg1, Ret2 > second(const Function1< Arg2, Ret2 > b2d, const F< Arg1, Arg2 >& fab)
  {
    const Function1<Arg1, Arg1> a2c = id<Arg1>;
    return BiFunctor< Function1< Arg1, Arg1 >, Function1< Arg2, Ret2 >, F< Arg1, Arg2 >, F< Arg1, Ret2 > >::bimap(a2c, b2d, fab);
  }

  // promap :: (c -> a) -> (b -> d) -> f a b -> f c d
  template < template < class... > class F, class Ret1, class Arg1, class Ret2, class Arg2 >
  F< Ret1, Ret2 > promap(const Function1< Ret1, Arg1 > c2a, const Function1< Arg2, Ret2 > b2d, const F< Arg1, Arg2 >& fab)
  {
    return ProFunctor< Function1< Ret1, Arg1 >, Function1< Arg2, Ret2 >, F< Arg1, Arg2 >, F< Ret1, Ret2 > >::promap(c2a, b2d, fab);
  }

  // Alias to promap<>() = dimap :: (c -> a) -> (b -> d) -> f a b -> f c d
  template < template < class... > class F, class Ret1, class Arg1, class Ret2, class Arg2 >
  F< Ret1, Ret2 > dimap(const Function1< Ret1, Arg1 > c2a, const Function1< Arg2, Ret2 > b2d, const F< Arg1, Arg2 >& fab)
  {
    return ProFunctor< Function1< Ret1, Arg1 >, Function1< Arg2, Ret2 >, F< Arg1, Arg2 >, F< Ret1, Ret2 > >::promap(c2a, b2d, fab);
  }

  // TODO: missing lmap<>() and rmap<>()

  // lmap :: (c -> a) -> f a b -> f c b
  template < template < class... > class F, class Ret1, class Arg1, class Arg2 >
  F< Ret1, Arg2 > lmap(const Function1< Ret1, Arg1 > c2a, const F< Arg1, Arg2 >& fab)
  {
    const Function1< Arg2, Arg2 > b2d = id<Arg2>;
    return ProFunctor< Function1< Ret1, Arg1 >, Function1< Arg2, Arg2 >, F< Arg1, Arg2 >, F< Ret1, Arg2 > >::promap(c2a, b2d, fab);
  }

  // rmap :: (b -> d) -> f a b -> f a d
  template < template < class... > class F, class Arg1, class Ret2, class Arg2 >
  F< Arg1, Ret2 > rmap(const Function1< Arg2, Ret2 > b2d, const F< Arg1, Arg2 >& fab)
  {
    const Function1< Arg1, Arg1 > c2a = id<Arg1>;
    return ProFunctor< Function1< Arg1, Arg1 >, Function1< Arg2, Ret2 >, F< Arg1, Arg2 >, F< Arg1, Ret2 > >::promap(c2a, b2d, fab);
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
