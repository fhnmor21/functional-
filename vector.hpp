#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "functor.hpp"
#include "applicative.hpp"
#include "monoid.hpp"
#include <vector>

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instances for std::vector
  template < class V >
  using Vector = std::vector< V, std::allocator< V > >;

  // ===
  // Functor
  template < class Ret, class Arg >
  struct Functor< Function1< Arg, Ret >, Vector< Arg >, Vector< Ret > >
  {
    static Vector< Ret > fmap(const Function1< Arg, Ret > a2b, const Vector< Arg >& fa)
    {
      Vector< Ret > fb;

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
  struct Applicative< Vector< Function1< Arg, Ret > >, Vector< Arg >, Vector< Ret > >
  {
    static Vector< Arg > apply(const Vector< Function1< Arg, Ret > >& fa2b, const Vector< Arg >& fa)
    {
      Vector< Ret > fb;

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
  struct Monoid< Vector<Val> >
  {
    static Vector<Val> associate(const Vector<Val>& m1, const Vector<Val>& m2)
    {
      Vector<Val> r;
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

    static Vector<Val> empty()
    {
      Vector<Val> r;
      return std::move(r);
    }
  };

} // end namespace FuncA2BalCpp

#endif // VECTOR_HPP
