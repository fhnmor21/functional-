#ifndef MONOID_UTILS_HPP
#define MONOID_UTILS_HPP

include "monoid.hpp"

namespace FunctionalCpp
{

  template < class M >
  M associate(const M& m1, const M& m2)
  {
    return Monoid<M>::associate(m1, m2);
  }

  template < class M >
  M empty()
  {
    return Monoid<M>::empty();
  }

} // end namespace FunctionalCpp

#endif // MONOID_UTILS_HPP


