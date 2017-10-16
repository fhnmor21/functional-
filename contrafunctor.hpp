#ifndef CONTRAFUNCTOR_HPP
#define CONTRAFUNCTOR_HPP

#include "function.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{
  //  ===
  // functor wrapper (static class)
  template < class ArrowB2A, class FunctorOf_A, class FunctorOf_B >
  struct ContraFunctor
  {
    // contramap :: (b -> a) -> f a -> f b
    static FunctorOf_B contramap(const ArrowB2A b2a, const FunctorOf_A& fa);
  };

} // end namespace FunctionalCpp

#endif // CONTRAFUNCTOR_HPP
