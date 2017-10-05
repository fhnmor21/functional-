#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "functor.hpp"
#include <vector>

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::vector
  template < class V >
  using Vector = std::vector< V, std::allocator< V > >;

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

} // end namespace FuncA2BalCpp

#endif // VECTOR_HPP
