#ifndef MONOID_HPP
#define MONOID_HPP

namespace FunctionalCpp
{

  template < class M >
  struct Monoid
  {
    static M associate(const M& m1, const M& m2);
    static M empty();
  };
} // end namespace FunctionalCpp

#endif // MONOID_HPP
