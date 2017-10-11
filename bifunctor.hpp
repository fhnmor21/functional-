#ifndef BIFUNCTOR_HPP
#define BIFUNCTOR_HPP

#include "function.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{
  //  ===
  // functor wrapper (static class)
  template < class ArrowA2C, class ArrowB2D, class FunctorOf_AB, class FunctorOf_CD >
  struct BiFunctor
  {
    // bimap :: (a -> c) -> (b -> d) f a b -> f b c
    static FunctorOf_CD bimap(const ArrowA2C a2c, const ArrowB2D b2d, const FunctorOf_AB& fab);
  };
} // end namespace FunctionalCpp

#endif // BIFUNCTOR_HPP
