#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include <vector>
#include <algorithm>
#include "curry.hpp"
#include "type_utils.hpp"

namespace FunctionalCpp
{

  template <typename F, typename R, typename A1>
  struct FunctorTypes
  {
    using function = std::function<R(A1)>;
    using result = R;
    using arg = A1; // this should be the same as value
    using value = inner_type<F>; // this should be the same as arg
    // using is_mappable = std::is_same<arg, value>::value;

    using ifunctor = F;
    using ofunctor = rebind_type<F, result>;
  };

  // simple functor map
  template <typename F, typename R, typename A1>
  typename FunctorTypes<F, R, A1>::ofunctor&&
  fmap(typename FunctorTypes<F, R, A1>::function a2b_, typename FunctorTypes<F, R, A1>::ifunctor& fa_ );


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


  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::vector
  template <typename T, typename R, typename A1>
  typename FunctorTypes<std::vector<T>, R, A1>::ofunctor&&
  fmap<std::vector<T>, R, A1>(typename FunctorTypes<std::vector<T>, R, A1> fn_a2b, typename FunctorTypes<std::vector<T>, R, A1>::ifunctor& fa_ )
  {
    typename FunctorTypes<std::vector<T>, R, A1>::ofunctor fb_;

    std::transform(fa_.begin(), fa_.end(), fb_.begin(), fn_a2b);
    return fb_;
  }


} // end namespace FunctionalCpp

#endif // FUNCTOR_HPP
