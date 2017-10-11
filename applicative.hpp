#ifndef APPLICATIVE_HPP
#define APPLICATIVE_HPP

#include "function.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{
  //  ===
  // Applicative wrapper (static class)
  template < class FunctorOf_A2B, class FunctorOf_A, class FunctorOf_B >
  struct Applicative : Functor< FunctorOf_A2B, FunctorOf_A, FunctorOf_B >
  {
    // apply :: f (a -> b) -> f a -> f b
    static FunctorOf_B apply(const FunctorOf_A2B& fa2b, const FunctorOf_A& fa);
  };

} // end namespace FunctionalCpp

#endif // APPLICATIVE_HPP
