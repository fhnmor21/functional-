#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "curry.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{
  //  ======
  // functor wrapper (static class)
  template < class ArrowA2B,
             class FunctorOf_A,
             class FunctorOf_B>
  struct Functor
  {
    // fmap :: (a -> b) -> f a -> f b
    static FunctorOf_B
    fmap(const ArrowA2B a2b,
         const FunctorOf_A& fa);
  };

  // template function
  template < template<class...> class F,
             class Ret,
             class Arg >
  F<Ret> fmap(const Function1<Arg, Ret> a2b,
              const F<Arg>& fa)
  {
    return Functor<Function1<Arg, Ret>, F<Arg>, F<Ret>>::fmap(a2b, fa);
  }


  //  ======
  // Applicative wrapper (static class)
  template < class FunctorOf_A2B,
             class FunctorOf_A,
             class FunctorOf_B >
  struct Applicative : Functor< FunctorOf_A2B, FunctorOf_A, FunctorOf_B >
  {
    // apply :: f (a -> b) -> f a -> f b
    static FunctorOf_B apply(const FunctorOf_A2B& fa2b, const FunctorOf_A& fa);
  };

  // template function
  template < template <class...> class F,
             class Ret,
             class Arg >
  F<Ret> apply(const F<Function1<Arg, Ret>>& fa2b,
               const F<Arg>& fa)
  {
    return Applicative<F<Function1<Arg,Ret>>, F<Arg>, F<Ret>>::apply(fa2b, fa);
  }

} // end namespace FunctionalCpp

#endif // FUNCTOR_HPP
