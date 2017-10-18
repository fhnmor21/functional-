#ifndef LIST_HPP
#define LIST_HPP

#include "functor.hpp"
#include "applicative.hpp"
#include "monoid.hpp"
#include <list>

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::vector
  template < class V >
  using List = std::list< V, std::allocator< V > >;

  // ===
  // Functor
  template < class Ret, class Arg >
  struct Functor< Function1< Arg, Ret >, List< Arg >, List< Ret > >
  {
    static List< Ret > fmap(const Function1< Arg, Ret > a2b, const List< Arg >& fa)
    {
      List< Ret > fb;

      for(auto& oldVal : fa)
      {
        auto newVal = a2b(oldVal);
        fb.push_back(newVal);
      }
      return std::move(fb);
    }
  };

  // ===
  // Applicative
  template < class Ret, class Arg >
  struct Applicative< List< Function1< Arg, Ret > >, List< Arg >, List< Ret > >
  {
    static List< Arg > apply(const List< Function1< Arg, Ret > >& fa2b, const List< Arg >& fa)
    {
      List< Ret > fb;

      for(auto& a2b : fa2b)
      {
        for(auto& oldVal : fa)
        {
          auto newVal = a2b(oldVal);
          fb.push_back(newVal);
        }
      }
      return std::move(fb);
    }
  };

  // ===
  // Monoid
  template <class Val>
  struct Monoid< List<Val> >
  {
    static List<Val> associate(const List<Val>& m1, const List<Val>& m2)
    {
      List<Val> r;
      for(auto & i: m1)
        {
          r.push_back(i);
        }
      for(auto & j: m2)
        {
          r.push_back(j);
        }

      return std::move(r);
    }

    static List<Val> empty()
    {
      List<Val> r;
      return std::move(r);
    }
  };


} // end namespace FuncA2BalCpp

#endif // LIST_HPP
