#ifndef CURRY_HPP
#define CURRY_HPP

#include <memory>
#include <utility>
#include <functional>
#include "type_utils.hpp"

namespace FunctionalCpp
{

  // BASED ON: www.github.com/andre-bergner/funky

  // function wrapper
  template <typename R, typename... Args>
  struct FuncWrap
  {
    // types
    //using Function = std::function<R(Args...)>;
    using Function = R(*)(Args...);
    template <size_t N>
    struct getArg
    {
      using Type = get_type<N, Args...>;
    };

    /*
    FuncWrap (Function&& func)
      : fn(func){}
    */

    FuncWrap (R (*func)(Args...))
      : fn(Function(func)) {}

    R operator()(Args&&... as)
    {
      return fn( std::forward<Args>(as)... );
    }

    const Function& fn;
  };

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
      return dispatch( call_succeed{} , std::get<Ns>(args_)... , std::forward<Other_args>(other_args)... );
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

  // template< typename R, typename A1, typename... Args, typename... BoundArgs >
  // auto curry( std::function<R(A1, Args...)>&& func , BoundArgs&&... boundArgs )
  // {
  //   return PartialFn<std::function<R(A1, Args...)>, BoundArgs...>( func , std::forward<BoundArgs>(boundArgs)... );
  // }

  template <typename R, typename... Args>
  auto functionWrapper(R(*f)(Args...))
  {
    return FuncWrap<R, Args...>(f);
  }


} // end nasmespace FunctionalCpp

#endif

