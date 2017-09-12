#ifndef CURRY_HPP
#define CURRY_HPP

#include <memory>
#include <utility>
#include <functional>
#include "type_utils.hpp"

namespace FunctionalCpp
{

  // LOOSELY BASED ON: www.github.com/andre-bergner/funky

  // function type utils
  template <typename Ret, typename... Args>
  struct FuncTypes
  {
    // general types
    using Function = Ret(*)(Args...);

    // **************************************************
    // function wrapper object
    struct Wrapper
    {
      // types
      static size_t constexpr argsNum = sizeof...(Args);
      // template <size_t N>
      // struct getArg
      // {
      //   using Type = get_type<N, Args...>;
      // };

      // Ctor
      Wrapper (const Function func)
      : fn(func) {}

      // Operators
      Ret operator()(Args&&... as)
      {
        return fn( std::forward<Args>(as)... );
      }

      // Data
      const Function& fn;
    }; // end Wrapper

    // // **************************************************
    // // partial application function object - forward declaration
    // template < typename... BoundArgs >
    // struct Partial
    // {
    //   // Types and Statics
    //   using ArgVals = std::tuple< typename std::remove_reference<BoundArgs>::type... >;
    //   static size_t constexpr tplSize = std::tuple_size<typename std::remove_reference<ArgVals>::type>::value;

    //   // // Ctor
    //   // Partial(Function& f, BoundArgs&&... as)
    //   //   : func_m(f) 
    //   //   , args_m( std::forward<BoundArgs>(as)...)
    //   // {}

    //   void init(Function& f, BoundArgs&&... as)
    //   {
    //     func_m = Wrapper(f);
    //     args_m = ArgVals(std::forward<BoundArgs>(as)...);
    //   }

    // //private:
    //     // Data
    //     Wrapper func_m;
    //     ArgVals args_m;
    // }; // end Partial

  }; // end FuncTypes


  template <typename Ret, typename... Args>
  using FnWrap = typename FuncTypes<Ret, Args...>::Wrapper;

  template <typename Ret, typename... Args>
  auto funcWrapper(Ret(*f)(Args...))
  {
    // using FnWrap = typename FuncTypes<Ret, Args...>::Wrapper;
    return FnWrap<Ret, Args...>(f);
  }

  // **************************************************
  // partial application function object - forward declaration
  template <
    typename Ret, 
    typename... Args >
  struct Partial
  {
    // Types and Statics
    using Function = typename FuncTypes<Ret, Args...>::Function;
    using Wrapper = typename FuncTypes<Ret, Args...>::Wrapper;
    using ArgVals = std::tuple< typename std::remove_reference<Args>::type... >;
    static size_t constexpr tplSize = std::tuple_size<typename std::remove_reference<ArgVals>::type>::value;

    // Ctor
    Partial(Function& f, Args&&... as)
      : func_m(f) 
      , args_m( std::forward<Args>(as)...)
    {}

  //private:
      // Data
      Wrapper func_m;
      ArgVals args_m;
  }; // end Partial



  // // template <typename Ret, typename... Args>
  // // auto partial( Ret(*f)(Args...) )
  // template <typename Ret, typename... Args, typename... BoundArgs>
  // auto partial( Ret(*f)(Args...), BoundArgs&&... boundArgs )
  // {
  //   // using Func = typename FuncTypes<Ret, Args...>::Function;
  //   // FuncTypes<Ret, Args...> funcType;
  //   // typename FuncTypes<Ret, Args...>::Partial fnPartial(f, boundArgs...);
  //   typename FuncTypes<Ret, Args...>::Partial fnPartial;
    
  //   return std::move(fnPartial);
  //   // funcType::Partial<BoundArgs...> fnPartial();
  //   // return fnPartial.init( std::forward<Func>(f) );
  //   //, std::forward<BoundArgs>(boundArgs)... );
  //   // return FuncTypes<Ret, Args...>::Factory ( f );
  // }



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

} // end nasmespace FunctionalCpp

#endif

