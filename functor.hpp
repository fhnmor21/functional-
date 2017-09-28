#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "curry.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{
  //  ======
  // functor wrapper (static class)
  template < typename ArrowA2B,
             typename FunctorOf_A,
             typename FunctorOf_B>
  struct Functor
  {
    // fmap :: (a -> b) -> f a -> f b
    static FunctorOf_B
    fmap(const ArrowA2B a2b,
         const FunctorOf_A& fa);
  };

  // template function
  template < template<typename...> class F,
             typename Ret,
             typename Arg >
  F<Ret> fmap(const Function<Arg, Ret> a2b,
              const F<Arg>& fa)
  {
    return Functor<Function<Arg, Ret>, F<Arg>, F<Ret>>::fmap(a2b, fa);
  }


  //  ======
  // Applicative wrapper (static class)
  template < typename FunctorOf_A2B,
             typename FunctorOf_A,
             typename FunctorOf_B >
  struct Applicative : Functor< FunctorOf_A2B, FunctorOf_A, FunctorOf_B >
  {
    // apply :: f (a -> b) -> f a -> f b
    static FunctorOf_B apply(const FunctorOf_A2B& fa2b, const FunctorOf_A& fa);
  };

  // template function
  template < template <typename...> class F,
             typename Ret,
             typename Arg >
  F<Ret> apply(const F<Function<Arg, Ret>>& fa2b,
               const F<Arg>& fa)
  {
    return Applicative<F<Function<Arg,Ret>>, F<Arg>, F<Ret>>::apply(fa2b, fa);
  }

} // end namespace FuncA2BalCpp

#endif // FUNCTOR_HPP
