#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include "functor.hpp"

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  // instance for std::vector

  template < typename R, typename A1 >
  struct FunctorImpl<std::vector<A1>, R, A1>
  {
    static auto map(const typename Functor<std::vector<A1>, R, A1>::Function& a2b, const typename Functor<std::vector<A1>, R, A1>::iFunctor& fa)
    {
      typename  Functor<std::vector<A1>, R, A1>::oFunctor fb;
      for (auto& oldVal: fa)
      {
        auto newVal = a2b(oldVal);
        fb.push_back(newVal);
      }
      return std::move(fb);
    }
  };

} // end namespace FunctionalCpp

#endif // VECTOR_HPP
