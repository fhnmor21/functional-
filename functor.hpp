#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "curry.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{

  template <
    typename F,
    typename R,
    typename A1 >
  struct Functor
  {
    using Function = std::function<R(A1)>;
    using Result = R;
    using Value = A1; // this should be the same as value
    using iFunctor = F;//<Value>;
    using oFunctor = typename rebind_type<F, Result>::type;

    static auto map(const typename Functor<F, R, A1>::Function& a2b, const typename Functor<F, R, A1>::iFunctor& fa){}
  };

  // empty implementation
  // has to be overriden by every type of functor
  template <
    typename F,
    typename R,
    typename A1 >
  struct FunctorImpl : Functor<F, R, A1>
  {};


  // template function
  template <
    typename F,
    typename R,
    typename A1 >
  //auto fmap(typename Functor<F, R, A1>::Function a2b, typename Functor<F, R, A1>::iFunctor fa)
  auto fmap(const std::function<R(A1)>& a2b, F& fa)
  {
    return FunctorImpl<F, R, A1>::map(a2b, fa);
  }


  // template <typename F, typename R, typename A1, typename A2, typename... As>
  // struct ApplicativeTypes
  // {
  //   using function = Curry<R, A1, A2, As...>;
  //   using result = function_type::ReturnT;
  //   using arg = A1; // this should be the same as value_type
  //   using value = inner_type<F>; // this should be the same as arg_type
  //   using = std::is_same<arg, value>::value;

  //   using ifunctor = F;
  //   using ofunctor = rebind_type<F, result>;
  // };

  // // applicative functor map
  // template <typename F, typename R, typename A1, typename A2, typename... As>
  // ApplicativeTypes<F, R, A1, A2, As...>::ofunctor&&
  // fmap(ApplicativeTypes<F, R, A1, A2, As...>::function a2b_, ApplicativeTypes<F, R, A1, A2, As...>::ifunctor& fa_ );

} // end namespace FunctionalCpp

#endif // FUNCTOR_HPP
