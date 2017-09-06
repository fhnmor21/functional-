#include "curry.hpp"

#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

namespace FunctionalCpp
{

  // forward declaration
  template <typename F>
  struct functor_wrapper;

  // template <typename F>
  // struct applicative_wrapper;

  // Applicative types
  template <typename F, typename R, typename A1, typename... Args >
  struct Applicative
  {
    using Fn = typename Curry<R, Args>;
    using Ret = typename Curry<R, A1, Args>::ReturnT;
    //using Type = applicative_wrapper<F>;
  };

  // Functor types and functions
  template <typename F>
  struct Functor
  {
    // Types
    using Type = functor_wrapper<F>;
    using Fn = std::function<T::type(T::type)>;

    // simple functor map
    static Type&& map(Fn a2b_, Type& fa_);

    // applicative functor map
    template <typename R, typename A1, typename... Args >
    static Applicative<F, F, A1, Args>::Ret&& map(Applicative<F, F, A1, Args>::Fn a2b_, Type& fa_);
  };

} // end namespace FunctionalCpp

#endif // FUNCTOR_HPP
