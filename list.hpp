#ifndef LIST_HPP
#define LIST_HPP

#include <list>
#include "functor.hpp"

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::vector
  template <class V>
  using List = std::list<V, std::allocator<V>>;

  template < class Ret,
             class Arg >
  struct Functor<Function1<Arg, Ret>, List<Arg>, List<Ret>>
  {
    static List<Ret> fmap(const Function1<Arg, Ret> a2b,
                          const List<Arg>& fa)
    {
      List<Ret> fb;

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
  struct Applicative<List<Function1<Arg, Ret>>, List<Arg>, List<Ret>>
  {
    static List<Arg> apply(const List<Function1<Arg, Ret>>& fa2b,
                            const List<Arg>& fa)
    {
      List<Ret> fb;

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

#endif // LIST_HPP
