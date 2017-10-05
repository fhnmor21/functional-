
#ifndef TYPE_UTILS_HPP
#define TYPE_UTILS_HPP

#include <cstddef>
#include <type_traits>

namespace FunctionalCpp
{
  ///
  // inner type of any parametric class
  template < class T >
  struct inner_type
  {
    using type = T;
  };

  template < template < class > class Tt, class T >
  struct inner_type< Tt< T > >
  {
    using type = T;
  };

  template < template < class, class... > class Tt, class T, class... Ts >
  struct inner_type< Tt< T, Ts... > >
  {
    using type = T;
  };

  ///
  // rebind a context or container to another parameter
  template < class Cp, class T >
  struct rebind_type
  {
    using type = Cp;
  };

  template < template < class > class Cp, class U, class T >
  struct rebind_type< Cp< T >, U >
  {
    using type = Cp< U >;
  };

  template < template < class... > class Cp, class U, class T, class... Ts >
  struct rebind_type< Cp< T, Ts... >, U >
  {
    using type = Cp< U, Ts... >;
  };

  ///
  // try to find the value type from containers
  template < class C >
  struct value_type
  {
    typedef typename C::value_type type;
  };

  template < class T, int N >
  struct value_type< T[N] >
  {
    typedef T type;
  };

  template < class T >
  struct value_type< T* >
  {
    typedef T type;
  };

  ///
  // nth element from an template arguments pack
  template < std::size_t N, class T, class... Ts >
  struct get_type
  {
    using type = typename get_type< N - 1, Ts... >::type;
  };

  template < class T, class... Ts >
  struct get_type< 0, T, Ts... >
  {
    using type = T;
  };

} // end namespace FunctionalCpp

#endif // TYPE_UTILS_HPP
