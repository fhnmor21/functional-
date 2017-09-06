
#ifndef MONOID_HPP
#define MONOID_HPP

namespace FunctionalCpp
{

  template <typename M>
  struct Monoid
  {
    static M&& associate (M m1, M m2);
    static M&& identity ();
  };

}; // end namespace FunctionalCpp


template <typename M>
M&& concat ()
{
  // TODO
}

#endif // MONOID_HPP
