#include "curry.hpp"
#include <vector>


#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

namespace FunctionalCpp
{

  template <typename A, typename R>
  using mappingFn = std::function<R(A)>;

  template <typename F>

  // forward declaration
  template <typename F>
  struct functor_wrapper;

  // template <typename F>
  // struct applicative_wrapper;

  // Applicative types
  template <typename F, typename R, typename A1, typename... Args >
  struct ApplicativeBase
  {
    using Fn = typename Curry<R, A1, Args>;
    using Ret = typename Curry<R, A1, Args>::ReturnT;
    //using Type = applicative_wrapper<F>;
  };

  // Functor types and functions
  template <typename F>
  struct FunctorBase
  {
    // Types
    using Type = functor_wrapper<F>;
    using Fn = std::function<T::type(T::type)>;
  };

  template <typename F>
  struct Functor : FunctorBase<F>
    // simple functor map
    static Type&& map(Fn a2b_, Type& fa_);

    // applicative functor map
    //    template <typename R, typename A1, typename... Args >
    //static Applicative<F, F, A1, Args>::Ret&& map(Applicative<F, F, A1, Args>::Fn a2b_, Type& fa_);
  };

  // local implementations
  using namespace std;
  template <typename ElementType>
  struct functor_wrapper< vector<ElementType> >
  {
    using Type = ElementType;
  };

  template <typename ElementType>
  struct Functor< vector<ElementType> > : FunctoBase< vector<ElementType> >
  {
    static Type&& map(Fn a2b_, Type& fa_);
  }





} // end namespace FunctionalCpp

#endif // FUNCTOR_HPP
