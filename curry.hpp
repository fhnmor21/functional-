#ifndef CURRY_HPP
#define CURRY_HPP

#include <memory>
#include "invoke.hpp"


namespace FunctionalCpp
{

  // BASED ON: www.github.com/andre-bergner/funky

  // forward declarion of currying function
  template < typename Function , typename... Args >
  auto curry( Function&& f , Args&&... args );

  // partial application object function
  template < typename Function , typename... OlgArgs >
  struct PartialFn
  {
  private:

    // Data
    Function const & func_;
    std::tuple< typename std::remove_reference<OlgArgs>::type... >   args_;


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
    PartialFn( Function const &f , OlgArgs&&... as )
    : func_(f) , args_( std::forward<OlgArgs>(as)...) {}

    // function object operator
    template < typename NewArg >
    auto operator()( NewArg&& newArg ) 
    {
      return  call( std::make_index_sequence<sizeof...(OlgArgs)>()
                  , std::forward<NewArg>(newArg) );
    }

  };

  // currying function
  template< typename Function , typename... Args >
  auto curry( Function&& f , Args&&... boundArgs )
  {
    return PartialFn<Function, Args...>( f , std::forward<Args>(boundArgs)... );
  }

} // end nasmespace FunctionalCpp

#endif

