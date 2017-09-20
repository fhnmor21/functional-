#ifndef CURRY_HPP
#define CURRY_HPP

#include "type_utils.hpp"
#include "invoke.hpp"

namespace FunctionalCpp
{
  // **************************************************
  // simple function wrapper object
  // with extras: Function type_id & numOfArgs & Nth Arg Type
  template <typename Ret, typename... Args>
  struct FnWrapper
  {
    // types
    static size_t constexpr numOfArgs = sizeof...(Args);
    using Function = Ret(*)(Args...);
    template <size_t N>
    using getArg = get_type<N, Args...>;

    // Ctor
    FnWrapper (Function func)
    : fn(func) {}

    // Operators
    Ret operator()(Args&&... as) const
    {
      return fn( std::forward<Args>(as)... );
    }

    const Function& operator~() const
    {
      return fn;
    }

  private:
    // Data
    Function fn;
  }; // end Wrapper

  // factory function for FN Wrapper
  template <typename Ret, typename... Args>
  auto fnWrapper(Ret(*f)(Args...))
  {
    return FnWrapper<Ret, Args...>(f);
  }

  // **************************************************
  // LOOSELY BASED ON: www.github.com/andre-bergner/funky
  // **************************************************

  // SFINAE trick to deal with incomplete set of arguments
  struct call_failed  {};
  struct call_succeed {};

  // forward declaration
  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto curry( Ret(*f)(FnArgs...), BoundArgs&&... boundArgs );

  // **************************************************
  // partial application function object - forward declaration
  template <
    typename Tpl,
    typename Ret,
    typename... FnArgs >
  struct FnPartial
  {
  public:
    // Types and Statics
    using Wrapper = FnWrapper<Ret, FnArgs...>;
    using Function = typename Wrapper::Function;
    static std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
    using NewArg = typename get_type<tSize, FnArgs...>::type;
    static constexpr bool lastRec = 0 == (FnWrapper<Ret, FnArgs...>::numOfArgs - tSize -1);
    using dispatcher = typename std::conditional< lastRec, call_succeed, call_failed >::type;

    // Ctor
    FnPartial(Function& f, Tpl& boundArgs)
      : func_m(fnWrapper(f))
      , boundArgs_m(boundArgs)
    {}

    // Operators
    auto operator()(NewArg&& a)
    {
      return call(std::make_index_sequence<tSize>(), std::forward<NewArg>(a));
    }

    const Wrapper& operator~() const
    {
      return func_m;
    }

  private:
    // Data
    Wrapper func_m;
    Tpl boundArgs_m;

    // dispacther function for correct numbers of arguments
    template < typename... CompArgs >
    Ret dispatch( call_succeed , CompArgs&&... compArgs ) const
    {
      return  (~func_m)( std::forward<CompArgs>(compArgs)... );
    }

    // dispatch function for incomplete set of arguments
    template < typename... CompArgs >
    auto dispatch( call_failed , CompArgs&&... compArgs ) const
    {
      return curry( (~func_m), std::forward<CompArgs>(compArgs)... );
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
  auto curry( Ret(*f)(FnArgs...), BoundArgs&&... boundArgs )
  {
    using BoundArgsTpl = std::tuple<typename std::remove_reference<BoundArgs>::type...>;
    BoundArgsTpl argsTpl(boundArgs...);
    return FnPartial<BoundArgsTpl, Ret, FnArgs...>(f, argsTpl);
  }

  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto curry( std::function<Ret(FnArgs...)> f, BoundArgs&&... boundArgs )
  {
    using BoundArgsTpl = std::tuple<typename std::remove_reference<BoundArgs>::type...>;
    BoundArgsTpl argsTpl(boundArgs...);
    return FnPartial<BoundArgsTpl, Ret, FnArgs...>(f.target< Ret(*f)(FnArgs...) >, argsTpl);
  }

} // end nasmespace FunctionalCpp

#endif

