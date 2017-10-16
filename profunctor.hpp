#ifndef PROFUNCTOR_HPP
#define PROFUNCTOR_HPP

#include "function.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{
  //  ===
  // functor wrapper (static class)
  template < class ArrowC2A, class ArrowB2D, class FunctorOf_AB, class FunctorOf_CD >
  struct ProFunctor
  {
    // promap :: (c -> a) -> (b -> d) f a b -> f c d
    static FunctorOf_CD promap(const ArrowC2A c2a, const ArrowB2D b2d, const FunctorOf_AB& fab);
  };
} // end namespace FunctionalCpp

#endif // PROFUNCTOR_HPP
