#ifndef CURRY_HPP
#define CURRY_HPP

#include "type_utils.hpp"
#include "invoke.hpp"

namespace FunctionalCpp
{
  // **************************************************
  // simple function wrapper object
  // with extras: Functio type_id & numOfArgs & Nth Arg Type
  template <typename Ret, typename... Args>
  struct FnWrapper
  {
    // types
    static size_t constexpr numOfArgs = sizeof...(Args);
    using Function = Ret(*)(Args...);
    template <size_t N>
    struct getArg
    {
      using Type = get_type<N, Args...>;
    };

    // Ctor
    FnWrapper (Function func)
    : fn(func) {}

    // Operators
    Ret operator()(Args&&... as)
    {
      return fn( std::forward<Args>(as)... );
    }

    // Data
    Function fn;
  }; // end Wrapper

  // factory function for FN Wrapper
  template <typename Ret, typename... Args>
  auto fnWrapper(Ret(*f)(Args...))
  {
    return FnWrapper<Ret, Args...>(f);
  }

  // SFINAE trick to deal with incomplete set of arguments
  struct call_failed  {};
  struct call_succeed {};


  // **************************************************
  // partial application function object - forward declaration
  // LOOSELY BASED ON: www.github.com/andre-bergner/funky
  template <
    typename Tpl,
    typename Ret,
    typename... FnArgs >
  struct FnPartial
  {
    // Types and Statics
    using Function = typename FnWrapper<Ret, FnArgs...>::Function;
    using Wrapper = FnWrapper<Ret, FnArgs...>;
    static std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
    static constexpr bool lastRec = 0 == (FnWrapper<Ret, FnArgs...>::numOfArgs - tSize -1);
    using dispatcher = typename std::conditional< lastRec, call_succeed, call_failed >::type;

  //private:
    // Data
    Wrapper func_m;
    Tpl boundArgs_m;

  //public:
    // Ctor
    FnPartial(Function& f, Tpl& boundArgs)
      : func_m(fnWrapper(f))
      , boundArgs_m(boundArgs)
    {}

    // dispacther function for correct numbers of arguments
    template < typename ArgsTpl >
    Ret dispatch( call_succeed , ArgsTpl&& argsTpl ) const
    {
      return Tuple::Vals::invoke(func_m.fn, argsTpl);
    }
    // dispatch function for incomplete set of arguments
    template < typename ArgsTpl >
    auto dispatch( call_failed , ArgsTpl&& argsTpl ) const
    {
      return FnPartial<Tpl, Ret, FnArgs...>::partial( std::forward<ArgsTpl>(argsTpl), func_m.fn  );
    }

   // Operators
    template <typename NewArg>
    auto operator()(NewArg&& a)
    {
      auto newArgsTpl = std::tuple_cat(boundArgs_m, std::tuple<NewArg>(std::forward<NewArg>(a)));
      dispatcher condition{};
      return dispatch(condition, newArgsTpl);
    }

    // recursive static factories
    //=========================================================================
    template <typename... BoundArgs>
    static auto partial( Ret(*f)(FnArgs...), BoundArgs&&... boundArgs )
    {
      Tpl argsTpl(boundArgs...);
      return FnPartial<Tpl, Ret, FnArgs...>(f, argsTpl);
    }

    template < typename BoundArgsTpl >
    static auto partial( BoundArgsTpl&& argsTpl, Ret(*f)(FnArgs...) )
    {
      return FnPartial<BoundArgsTpl, Ret, FnArgs...>(f, argsTpl);
    }

  }; // end Partial

  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto curry( Ret(*f)(FnArgs...), BoundArgs&&... boundArgs )
  {
    using BoundArgsTpl = std::tuple<BoundArgs...>;
    return FnPartial<BoundArgsTpl, Ret, FnArgs...>::partial(f, boundArgs...);
  }

} // end nasmespace FunctionalCpp

#endif

