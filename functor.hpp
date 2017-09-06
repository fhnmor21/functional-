#include "curry.hpp"

#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

namespace FunctionalCpp
{

  template <typename F>
  struct functor_wrapper;

  template <typename F>
  struct applicative_wrapper;

  template <typename F>
  struct Functor
  {
    using FunctorT = functor_wrapper<F>;
    using FunctorFn = std::function<FunctorT::type(FunctorT::type)>;

    template <typename... Args>
    using applicativeR = std::function<>;

    template <typename Arg, typename... Args>
    using ApplicativeF = std::function<>;

    static FunctorT&& map(FuntoorT& functor_, std::function<::type(T::type)>);
    template<typename... Args>

    static F&& map(F& functor_, std::function<T::type(T::type, Args...)>);
  };

}; // end namespace FunctionalCpp

#endif // FUNCTOR_HPP
