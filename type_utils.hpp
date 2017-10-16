
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
  struct type_empty_list
  {
    constexpr static size_t size = 0;
  };

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
  // apppend a type to a type list
  template <class Append, class TypeList, size_t N = TypeList::size>
  struct append_type
  {
    using list = typename append_type<Append, typename TypeList::tail, N-1>::list;
    using head = typename list::head;
    using tail = typename list::tail;
    constexpr static size_t size = 1+ TypeList::size;
  };

  template <class Append, class TypeList>
  struct append_type<Append, TypeList, 1>
  {
    using list = TypeList;
    using head = typename TypeList::head;
    using tail = type_list<Append>;
    constexpr static size_t size = 1+ TypeList::size;
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
  // drop remove the first N elements from a type list

  template <size_t N, class TypeList>
  struct drop_types
  {
    static_assert(N < TypeList::size, "drop_types<N, TypeList>: Dropping more types than the size of the list.");
    using list = typename drop_types<N-1, typename TypeList::tail>::list;
    using head = typename list::head;
    using tail = typename list::tail;
    constexpr static size_t size = list::size;
  };

  template <class TypeList>
  struct drop_types<0, TypeList>
  {
    using list = TypeList;
    using head = typename TypeList::head;
    using tail = typename TypeList::tail;
    constexpr static size_t size = TypeList::size;
  };


  ///
  // take the N first elements from a type list
  template <size_t N, class TypeList>
  struct take_types
  {
    static_assert(N < TypeList::size, "taking_types<N, TypeList>: Taking more types than the size of the list.");
    using list = TypeList;
    using head = typename TypeList::head;
    using tail = typename take_types<N - 1, typename TypeList::tail>::list;
    constexpr static size_t size = N;
  };

  template <class TypeList>
  struct take_types<0, TypeList>
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
