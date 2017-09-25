#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "curry.hpp"
#include "type_utils.hpp"

namespace FuncA2BalCpp
{
  // functor wrapper (static class)
  template < typename F,
             typename R,
             typename A1 >
  struct Functor
  {
    using FuncA2B = Function<A1,R>;
    using FunctorA = F;
    using FunctorB = typename rebind_type<F, R>::type;

    static auto map(const typename Functor<F, R, A1>::FuncA2B& a2b, const typename Functor<F, R, A1>::FunctorA& fa){}
  };

  // Functor empty implementation
  // has to be overriden by every type of functor
  template < typename F,
             typename R,
             typename A1 >
  struct FunctorImpl : Functor<F, R, A1>
  {};


  // template function
  template < typename F,
             typename R,
             typename A1 >
  auto fmap(const Function<A1,R>& a2b, F& fa)
  {
    return FunctorImpl<F, R, A1>::map(a2b, fa);
  }

  /*
  // Applicative wrapper (static class)
  template < typename F,
             typename R,
             typename A1,
             typename A2,
             typename... As>
  struct Applicative : Functor<F, R, A1>
  {
    using FuncA2B  = typename Recursive<R, A1, A2, As...>::type;
    using Result = typename Recursive<R, A2, As...>::type;
    using aFunctor = typename rebind_type<F, Result>::type;

    static auto map(const typename Applicative<F, R, A1, A2, As...>::FuncA2B& a2b, const typename Applicative<F, R, A1, A2, As...>::FunctorA& fa){}
  };

  // applicative empty implementation
  template < typename F,
             typename R,
             typename A1,
             typename A2,
             typename... As >
  struct ApplicativeImpl : Applicative<F, R, A1, A2, As...>
  {};

  // template function
  template < typename F,
             typename R,
             typename A1,
             typename A2,
             typename... As >
  auto fmap(const Recursive<R, A1, A2, As...>& a2b, F& fa)
  {
    return ApplicativeImpl<F, R, A1, A2, As...>::map(a2b, fa);
  }
  */

} // end namespace FuncA2BalCpp

#endif // FUNCTOR_HPP
