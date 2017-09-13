#ifndef CURRY_HPP
#define CURRY_HPP

#include <memory>
#include <utility>
#include <functional>
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


  // **************************************************
  // partial application function object - forward declaration
  // LOOSELY BASED ON: www.github.com/andre-bergner/funky
  template <
    typename Tpl,
    typename Ret,
    typename... Args >
  struct FnPartial
  {
    /*
    template < typename... ValArgs >
    auto dispatch_( ValArgs&&... args ) const -> Ret
    {
      return  func_m( std::forward<ValArgs>(args)... );
    }

    // calling to the function forwarding arguments
    template < std::size_t... Ns , typename ArgsTpl >
    auto call_( std::index_sequence<Ns...> , ArgsTpl&& tpl )
    {
      return dispatch_( std::get<Ns>(tpl)... );
    }
    */

    // Types and Statics
    using Function = typename FnWrapper<Ret, Args...>::Function;
    using Wrapper = FnWrapper<Ret, Args...>;
    static std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;

    // Ctor
    FnPartial(Function& f, Tpl& boundArgs)
      : func_m(fnWrapper(f))
      , boundArgs_m(boundArgs)
    {}

    // Operators
    template <typename NewArg>
    auto operator()(NewArg&& a)
    {
      auto argVals = std::tuple_cat(boundArgs_m, std::tuple<NewArg>(std::forward<NewArg>(a)));
      return Tuple::Vals::invoke(func_m.fn, argVals);
    }

  //private:
    // Data
    Wrapper func_m;
    Tpl boundArgs_m;

  }; // end Partial

  template <typename Ret, typename... FnArgs, typename... BoundArgs>
  auto partial( Ret(*f)(FnArgs...), BoundArgs&&... boundArgs )
  {
    using BoundArgsTpl = std::tuple<BoundArgs...>;
    BoundArgsTpl bound(boundArgs...);
    return FnPartial<BoundArgsTpl, Ret, FnArgs...>(f, bound);
  }










  // FROM: www.github.com/andre-bergner/funky

  // forward declarion of currying function
  template < typename Function , typename... BoundArgs >
  auto curry( Function&& f , BoundArgs&&... boundArgs );

  // partial application object function
  template < typename Function , typename... OldArgs >
  struct PartialFn
  {
  private:
    // Types and Statics
    using ArgVals = std::tuple< typename std::remove_reference<OldArgs>::type... >;
    static std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<ArgVals>::type>::value;
    // using NewArgN = typename get_type< tSize-1, OldArgs... >::type;

    // Data
    Function const & func_;
    ArgVals args_;

    // ************************************************************************
    // SFINAE trick to deal with incomplete set of arguments
    struct call_failed {};
    struct call_succeed : call_failed {};

    // dispacther function for correct numbers of arguments
    template < typename... Args >
    auto dispatch( call_succeed , Args&&... args ) const -> decltype( func_(args...) )
    {
      return  func_( std::forward<Args>(args)... );
    }

    // dispatch function for incomplete set of arguments
    template < typename... Args >
    auto dispatch( call_failed , Args&&... args ) const -> decltype( curry( func_ , std::forward<Args>(args)... ) )
    {
      return  curry( func_ , std::forward<Args>(args)... );
    }

    // calling to the function forwarding arguments
    template < std::size_t... Ns , typename... Other_args >
    auto call( std::index_sequence<Ns...> , Other_args&&... other_args )
    {
      return dispatch( call_succeed{} ,  std::get<Ns>(args_)... , std::forward<Other_args>(other_args)... );
    }

  public:
    // Ctor
    PartialFn( Function const &f , OldArgs&&... as )
    : func_(f) , args_( std::forward<OldArgs>(as)...) {}

    // function object operator
    template < typename NewArg >
    auto operator()( NewArg&& newArg ) 
    {
      return  call( std::make_index_sequence<sizeof...(OldArgs)>()
                  , std::forward<NewArg>(newArg) );
    }

  };

  //currying function
  template< typename Function , typename... BoundArgs >
  auto curry( Function&& func , BoundArgs&&... boundArgs )
  {
    return PartialFn<Function, BoundArgs...>( func , std::forward<BoundArgs>(boundArgs)... );
  }

} // end nasmespace FunctionalCpp

#endif

