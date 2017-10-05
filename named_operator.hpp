#ifndef NAMED_OPERATOR_HPP
#define NAMED_OPERATOR_HPP

//*****************************************************************************************
//
//
//
//      Copied originally ffrom
//      https://github.com/klmr/named-operator
//      Licensed under the Apache License, Version 2.0 (the "License");
//
//
//
//*****************************************************************************************

#include <utility>

namespace FunctionalCpp
{

  template < typename F >
  struct named_operator_wrapper
  {
    F f;
  };

  template < typename T, typename F >
  struct named_operator_lhs
  {
    F f;
    T& value;
  };

  template < typename T, typename F >
  inline named_operator_lhs< T, F > operator<(T& lhs, named_operator_wrapper< F > rhs)
  {
    return {rhs.f, lhs};
  }

  template < typename T, typename F >
  inline named_operator_lhs< T const, F > operator<(T const& lhs, named_operator_wrapper< F > rhs)
  {
    return {rhs.f, lhs};
  }

  template < typename T1, typename T2, typename F >
  inline auto operator>(named_operator_lhs< T1, F > const& lhs, T2 const& rhs)
      -> decltype(lhs.f(std::declval< T1 >(), std::declval< T2 >()))
  {
    return lhs.f(lhs.value, rhs);
  }

  template < typename T1, typename T2, typename F >
  inline auto operator>=(named_operator_lhs< T1, F > const& lhs, T2 const& rhs)
      -> decltype(lhs.value = lhs.f(std::declval< T1 >(), std::declval< T2 >()))
  {
    return lhs.value = lhs.f(lhs.value, rhs);
  }

  template < typename F >
  inline constexpr named_operator_wrapper< F > make_named_operator(F f)
  {
    return {f};
  }

} // namespace FunctionalCpp

#endif // endof  NAMED_OPERATOR_HPP
