
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

  ///
  // from a pack of varidic type create a type list
  struct type_empty_list{};

  template <class A1, class... As>
  struct type_list
  {
    using head = A1;
    using tail = type_list<As...>;
    constexpr static size_t size = 1 + sizeof...(As);
  };

  template <class A1>
  struct type_list<A1>
  {
    using head = A1;
    using tail = type_empty_list;
    constexpr static size_t size = 1;
  };

  ///
  // prepend a type to a type list
  template <class Prepend, class TypeList>
  struct prepend_type: type_list<Prepend>
  {
    using head = Prepend;
    using tail = TypeList;
    constexpr static size_t size = 1 + TypeList::size;
  };

  ///
  // drop remove the firn N elements from a type list
  namespace Impl_
  {
    template <size_t N, class TypeList>
    struct drop_types_
    {
      using list = typename drop_types_<N-1, typename TypeList::tail>::list;
    };

    template <class TypeList>
    struct drop_types_<1,TypeList>
    {
      using list = typename TypeList::tail;
    };
  }
  template <size_t N, class TypeList>
  struct drop_types
  {
    using head = typename Impl_::drop_types_<N, TypeList>::head;
    using tail = typename Impl_::drop_types_<N, TypeList>::tail;
    using size = typename Impl_::drop_types_<N, TypeList>::size;
  };

  ///
  // take the N first elements from a type list
  template <size_t N, class TypeList>
  struct take_types
  {
    using head = typename TypeList::head;
    using tail = typename take_types<N - 1, typename TypeList::tail>::list;
    constexpr static size_t size = N;
    using list = TypeList;
  };

  template <class TypeList>
  struct take_types<1, TypeList>
  {
    using head = typename TypeList::head;
    using tail = type_empty_list;
    constexpr static size_t size = 1;
  };

  ///
  // nth element from a type list
  template < size_t N, class TypeList >
  struct get_type_at
  {
    using type = typename get_type_at< N - 1, typename TypeList::tail >::type;
  };

  template < class TypeList>
  struct get_type_at< 0, TypeList >
  {
    using type = typename TypeList::head;
  };

} // end namespace FunctionalCpp

#endif // TYPE_UTILS_HPP
