#ifndef DICT_HPP
#define DICT_HPP

#include "bifunctor.hpp"
#include <map>

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::map
  template < class K, class V >
  using Dict = std::map< K, V >;

  template < class Ret1, class Arg1, class Ret2, class Arg2 >
  struct BiFunctor< Function1< Arg1, Ret1 >, Function1< Arg2, Ret2 >, Dict< Arg1, Arg2 >, Dict< Ret1, Ret2  > >
  {
    static Dict< Ret1, Ret2 > bimap(const Function1< Arg1, Ret1 > a2c, const Function1< Arg2, Ret2 > b2d, const Dict< Arg1, Arg2 >& fab)
    {
      Dict< Ret1, Ret2 > fcd;

      for(auto& oldItem : fab)
      {
        auto newKey = a2c(oldItem.first);
        auto newVal = b2d(oldItem.second);
        fcd[newKey] = newVal;
      }
      return std::move(fcd);
    }
  };

} // end namespace FuncA2BalCpp

#endif // DICT_HPP
