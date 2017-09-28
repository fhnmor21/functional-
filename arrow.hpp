#ifndef ARROW_HPP
#define ARROW_HPP

#include "type_utils.hpp"
#include "invoke.hpp"

namespace FunctionalCpp
{
template <typename A, typename B>
using Function = std::function<B(A)>;

namespace Impl_
{
  // **************************************************
  // recursive std::function return type for partial applications
  template < size_t N,
             typename Ret,
             typename... Args >
  struct Nested
  {};

  template < size_t N,
             typename Ret,
             typename Arg,
             typename... Args >
  struct Nested< N, Ret, Arg, Args... > : Nested< N-1, Ret, Args... >
  {};

  template < typename Ret,
             typename Arg,
             typename... Args >
  struct Nested< 0, Ret, Arg, Args... >
  {
  private:
  using R = typename Nested< 0, Ret, Args... >::type;
  public:
  using type = Function<Arg, R>;
  };

  template < typename Ret >
  struct Nested< 0, Ret >
  {
  public:
  using type = Ret;
  };
  } // end namespace Impl_

  template < typename Ret,
             typename... Args>
  using Curried = Impl_::Nested<0, Ret, Args...>;


} // end namespace FunctionalCpp

#endif // ARROW_HPP