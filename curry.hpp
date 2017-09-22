#ifndef CURRY_HPP
#define CURRY_HPP

#include "type_utils.hpp"
#include "invoke.hpp"

namespace FunctionalCpp
{
  // **************************************************
  // LOOSELY BASED ON: www.github.com/andre-bergner/funky
  // **************************************************

  // SFINAE trick to deal with incomplete set of arguments
  struct call_failed  {};
  struct call_succeed {};

  // forward declaration
  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto curry( Ret(f)(FnArgs...), BoundArgs&&... boundArgs );

  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto curry( std::function<Ret(FnArgs...)> f, BoundArgs&&... boundArgs );

  // **************************************************
  // std::function return type for partial applications
  template <
    size_t N,
    typename Ret,
    typename... Args >
  struct Return
  {};

  template <
    size_t N,
    typename Ret,
    typename Arg,
    typename... Args >
  struct Return< N, Ret, Arg, Args... > : Return< N-1, Ret, Args... >
  {};

  template <
    typename Ret,
    typename Arg,
    typename... Args >
  struct Return< 0, Ret, Arg, Args... >
  {
  private:
    using R = typename Return< 0, Ret, Args... >::type;
  public:
    using type = std::function< R(Arg) >;
  };

  template <
    typename Ret >
  struct Return< 0, Ret >
  {
  public:
    using type = Ret;
  };

  // **************************************************
  // partial application function object - forward declaration
  template <
    typename Tpl,
    typename Ret,
    typename... FnArgs >
  struct Partial
  {
  private:
    // Types and Statics
    static size_t constexpr numOfArgs = sizeof...(FnArgs);
    static std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
    static constexpr bool lastRec = ( 0 == (numOfArgs - tSize -1) );
    using dispatcher = typename std::conditional< lastRec, call_succeed, call_failed >::type;
    using FnWrapper = std::function<Ret(FnArgs...)>;
    using NewArg = typename get_type<tSize, FnArgs...>::type;
    using OpReturn = typename Return<tSize+1, Ret, FnArgs...>::type;
    using Function = std::function<OpReturn(NewArg)>;

  public:
    Partial(FnWrapper f, Tpl& boundArgs)
      : wrapper_m(f)
      , boundArgs_m(boundArgs)
    {
      fn_m = (std::function<OpReturn(NewArg)>(*this));
    }

    // Operators
    // ===
    // calls function object
    auto operator()(NewArg&& a)
    {
      return call(std::make_index_sequence<tSize>(), std::forward<NewArg>(a));
    }

    // returns function wrapper
    const FnWrapper& operator!() const
    {
      return wrapper_m;
    }

    // returns function wrapper
    const auto& operator~() const
    {
      return fn_m;
    }

  private:
    // Data
    FnWrapper wrapper_m;
    Tpl boundArgs_m;
    Function fn_m;

    // dispacther function for correct numbers of arguments
    template < typename... CompArgs >
    Ret dispatch( call_succeed , CompArgs&&... compArgs ) const
    {
      return  (wrapper_m)( std::forward<CompArgs>(compArgs)... );
    }

    // dispatch function for incomplete set of arguments
    template < typename... CompArgs >
    auto dispatch( call_failed , CompArgs&&... compArgs ) const
    {
      return curry( (wrapper_m), std::forward<CompArgs>(compArgs)... );
    }

    template < std::size_t... Ns , typename NewArg >
    auto call( std::index_sequence<Ns...> , NewArg&& newArg )
    {
      dispatcher condition{};
      return dispatch(  condition,
                        std::get<Ns>(boundArgs_m)... ,
                        std::forward<NewArg>(newArg) );
    }

  }; // end Partial

  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto curry( Ret(f)(FnArgs...), BoundArgs&&... boundArgs )
  {
    std::function<Ret(FnArgs...)>func = f;
    return curry(func, std::forward<BoundArgs>(boundArgs)...);
  }

  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto curry( std::function<Ret(FnArgs...)> f, BoundArgs&&... boundArgs )
  {
    using BoundArgsTpl = std::tuple<typename std::remove_reference<BoundArgs>::type...>;
    BoundArgsTpl argsTpl(boundArgs...);
    return Partial<BoundArgsTpl, Ret, FnArgs...>(f, argsTpl);
  }

} // end nasmespace FunctionalCpp

#endif

