#ifndef CURRY_HPP
#define CURRY_HPP

#include "arrow.hpp"

namespace FunctionalCpp
{
  // **************************************************
  // LOOSELY BASED ON: www.github.com/andre-bergner/funky
  // **************************************************

  // SFINAE trick to deal with incomplete set of arguments
  struct call_failed  {};
  struct call_succeed {};

  // forward declaration
  template <class Ret, class... FnArgs, class... BoundArgs>
  auto make_curried( Ret(f)(FnArgs...), BoundArgs&&... boundArgs );

  template <class Ret, class... FnArgs, class... BoundArgs>
  auto make_curried( std::function<Ret(FnArgs...)> f, BoundArgs&&... boundArgs );

  // **************************************************
  // partial application function object - forward declaration
  template <
    class Tpl,
    class Ret,
    class... FnArgs >
  struct Partial
  {
  private:
    // Statics
    static size_t constexpr numOfArgs = sizeof...(FnArgs);
    static std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
    static constexpr bool lastRec = ( 0 == (numOfArgs - tSize -1) );
    // Types
    using Dispatcher = typename std::conditional< lastRec, call_succeed, call_failed >::type;
    using FnWrapper  = std::function<Ret(FnArgs...)>;
    using NewArg     = typename get_type<tSize, FnArgs...>::type;
    using OpNested   = typename Impl_::Nested<tSize+1, Ret, FnArgs...>::type;
    using FnCurried  = typename Impl_::Nested<tSize, Ret, FnArgs...>::type;

  public:

    // Ctor
    Partial(FnWrapper f, Tpl& boundArgs)
      : wrapper_m(f)
      , boundArgs_m(boundArgs)
    {
      fn_m = (std::function<OpNested(NewArg)>(*this));
    }

    // returns nestleed function - curried
    const auto& curried() const
    {
      return fn_m;
    }

    // return flat args function - uncurried
    const FnWrapper& uncurried() const
    {
      return wrapper_m;
    }

    // Operators
    // ===

    // calls function object
    auto operator()(NewArg&& a)
    {
      return call(std::make_index_sequence<tSize>(), std::forward<NewArg>(a));
    }

    const auto& operator~() const
    {
      return this->curried();
    }

    const FnWrapper& operator!() const
    {
      return this->uncurried();
    }

  private:
    // Data
    FnWrapper wrapper_m;
    Tpl boundArgs_m;
    FnCurried fn_m;

    // dispacther function for correct numbers of arguments
    template < class... CompArgs >
    Ret dispatch( call_succeed , CompArgs&&... compArgs ) const
    {
      return  (wrapper_m)( std::forward<CompArgs>(compArgs)... );
    }

    // dispatch function for incomplete set of arguments
    template < class... CompArgs >
    auto dispatch( call_failed , CompArgs&&... compArgs ) const
    {
      return make_curried( (wrapper_m), std::forward<CompArgs>(compArgs)... );
    }

    template < std::size_t... Ns , class NewArg >
    auto call( std::index_sequence<Ns...> , NewArg&& newArg )
    {
      Dispatcher condition{};
      return dispatch(  condition,
                        std::get<Ns>(boundArgs_m)... ,
                        std::forward<NewArg>(newArg) );
    }

  }; // end Partial

  template <class Ret, class... FnArgs, class... BoundArgs>
  auto make_curried( Ret(f)(FnArgs...), BoundArgs&&... boundArgs )
  {
    std::function<Ret(FnArgs...)> func = f;
    return make_curried(func, std::forward<BoundArgs>(boundArgs)...);
  }

  template <class Ret, class... FnArgs, class... BoundArgs>
  auto make_curried( std::function<Ret(FnArgs...)> f, BoundArgs&&... boundArgs )
  {
    using BoundArgsTpl = std::tuple<typename std::remove_reference<BoundArgs>::type...>;
    BoundArgsTpl argsTpl(boundArgs...);
    return Partial<BoundArgsTpl, Ret, FnArgs...>(f, argsTpl);
  }

} // end nasmespace FunctionalCpp

#endif

