#ifndef DICT_HPP
#define DICT_HPP

#include "functor.hpp"
#include <map>

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::map
  template < class V >
  using Dict = std::map< const std::string, V >;

  template < class Ret, class Arg >
  struct Functor< Function1< Arg, Ret >, Dict< Arg >, Dict< Ret > >
  {
    static Dict< Ret > fmap(const Function1< Arg, Ret > a2b, const Dict< Arg >& fa)
    {
      Dict< Ret > fb;

      for(auto& oldVal : fa)
      {
        auto newVal = a2b(oldVal.second);
        fb[oldVal.first].newVal;
      }
      return std::move(fb);
    }
  };

  /* TBD ???
  template < class Ret, class Arg >
  struct Applicative< Dict< Function1< Arg, Ret > >, Dict< Arg >, Dict< Ret > >
  {
    static Dict< Arg > apply(const Dict< Function1< Arg, Ret > >& fa2b, const Dict< Arg >& fa)
    {
      Dict< Ret > fb;

      for(auto& a2b : fa2b)
      {
        for(auto& oldVal : fa)
        {
          auto newVal = a2b(oldVal.second);
          fb[oldVal.first].newVal;
        }
      }
      return std::move(fb);
    }
  };
  */

} // end namespace FuncA2BalCpp

#endif // DICT_HPP
