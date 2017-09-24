#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "curry.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{

  // functor wrapper (static class)
  template < typename F,
             typename R,
             typename A1 >
  struct Functor
  {
    using Function = std::function<R(A1)>;
    using iFunctor = F; //<Value>;
    using oFunctor = typename rebind_type<F, R>::type;

    static auto map(const typename Functor<F, R, A1>::Function& a2b, const typename Functor<F, R, A1>::iFunctor& fa){}
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
  auto fmap(const std::function<R(A1)>& a2b, F& fa)
  {
    return FunctorImpl<F, R, A1>::map(a2b, fa);
  }


  // applicative wrapper (static class)
  template < typename F,
             typename R,
             typename A1,
             typename A2,
             typename... As>
  struct Applicative : Functor<F, R, A1>
  {
    using Function  = typename Return<0,R, A1, A2, As...>::type;
    using Result = typename Return<0, R, A2, As...>::type;
    using aFunctor = typename rebind_type<F, Result>::type;

    static auto map(const typename Applicative<F, R, A1, A2, As...>::Function& a2b, const typename Applicative<F, R, A1, A2, As...>::iFunctor& fa){}
  };



} // end namespace FunctionalCpp

#endif // FUNCTOR_HPP
