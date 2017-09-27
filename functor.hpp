#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "curry.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{
  // functor wrapper (static class)
  template < typename FuncA2B,
             typename FunctorA,
             typename FunctorB>
  struct Functor
  {
    // fmap :: (a -> b) -> f a -> f b
    static FunctorB
    fmap(const FuncA2B a2b,
         const FunctorA& fa);
  };

  // template function
  template < template<typename...> class F,
             typename R,   // B
             typename A1 > // A
  F<R> fmap(const Function<A1, R> a2b,
            const F<A1>& fa)
  {
    return Functor<Function<A1, R>, F<A1>, F<R>>::fmap(a2b, fa);
  }

  /*
  // Applicative wrapper (static class)
  template < typename F,
             typename R,   // B
             typename A1 > // A
  struct Applicative : Functor< F, R, A1 >
  {
    // inherited types:
    // FunctorA = F<A1> : f a
    // FuncA2B = Function<A1,R> : (a -> b)
    // FunctorB =  F<R> : f b  [ rebind_type<F, R>::type ]

    // FunctorA2B = F< Function<A1,R> > : f (a -> b)
    using FunctorA2B = typename rebind_type<F, typename Applicative< F, R, A1 >::FuncA2B>::type;

    // apply :: f (a -> b) -> f a -> f b
    static auto apply(const typename Applicative<F, R, A1>::FunctorA2B& fa2b, const F& fa);
  };

  // Applicative empty implementation
  // has to be overriden by every type of functor
  template < typename F,
             typename R,
             typename A1 >
  struct ApplicativeImpl : Applicative<F, R, A1>
  {};

  // template function
  template < template <typename...> class F,
             typename R,
             typename A1 >
  auto
  apply(const F<Function<A1, R>>& fa2b,
        const F<A1>& fa)
  {
    return ApplicativeImpl<F<A1>, R, A1>::apply(fa2b, fa);
  }
  */
} // end namespace FuncA2BalCpp

#endif // FUNCTOR_HPP
