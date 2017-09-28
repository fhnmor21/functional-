#ifndef DICT_HPP
#define DICT_HPP

#include <map>
#include "functor.hpp"

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::map
  template <class V>
  using Dict = std::map<const std::string, V>; //, std::less<std::string>, std::allocator<std::pair<const std::string, V>>;

  template < class Ret,
             class Arg >
  struct Functor<Function<Arg, Ret>, Dict<Arg>, Dict<Ret>>
  {
    static Dict<Ret> fmap(const Function<Arg, Ret> a2b,
                          const Dict<Arg>& fa)
    {
      Dict<Ret> fb;

      for (auto& oldVal: fa)
      {
        auto newVal = a2b(oldVal);
        fb.push_back(newVal);
      }
      return std::move(fb);
    }
  };


  template < class Ret,
             class Arg >
  struct Applicative<Dict<Function<Arg, Ret>>, Dict<Arg>, Dict<Ret>>
  {
    static Dict<Arg> apply(const Dict<Function<Arg, Ret>>& fa2b,
                            const Dict<Arg>& fa)
    {
      Dict<Ret> fb;

      for(auto& a2b : fa2b)
      {
        for (auto& oldVal: fa)
        {
          auto newVal = a2b(oldVal);
          fb.push_back(newVal);
        }
      }
      return std::move(fb);
    }
  };

} // end namespace FuncA2BalCpp

#endif // DICT_HPP
