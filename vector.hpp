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
    static auto fmap(const typename Functor<std::vector<A1>, R, A1>::FuncA2B& a2b,
                     const typename Functor<std::vector<A1>, R, A1>::FunctorA& fa)
    {
      typename Functor<std::vector<A1>, R, A1>::FunctorB fb;

      for (auto& oldVal: fa)
      {
        auto newVal = a2b(oldVal);
        fb.push_back(newVal);
      }
      return std::move(fb);
    }
  };


  template < typename R,
             typename A1 >
  struct ApplicativeImpl<std::vector<A1>, R, A1>
  {
    static auto apply(const typename Applicative<std::vector<A1>, R, A1>::FunctorA2B& fa2b,
                      const typename Applicative<std::vector<A1>, R, A1>::FunctorA& fa)
    {
      typename Applicative<std::vector<A1>, R, A1>::FunctorB fb;

      for(auto& a2b : fa2b)
      {
        for (auto& oldVal: fa)
        {
          auto newVal = a2b(oldVal);
          fb.push_back(newVal);
        }
      }
    }
  };


  /*
  template < typename R, typename A1, typename... As >
  struct FunctorImpl<std::vector<A1>, R, A1, As...>
  {
    static auto map(const typename Functor<std::vector<A1>, R, A1, As...>::FuncA2B& a2b,
                    const typename Functor<std::vector<A1>, R, A1, As...>::FunctorA& fa)
    {
      typename  Functor<std::vector<A1>, R, A1, As...>::FunctorB fb;
      for (auto& oldVal: fa)
      {
        auto newVal = a2b(oldVal);
        fb.push_back(newVal);
      }
      return std::move(fb);
    }
  };
  */

  /*
  template < typename R,
             typename A1,
             typename A2,
             typename... As >
  struct ApplicativeImpl<std::vector<A1>, R, A1, A2, As...>
  {
    static auto map(const typename Applicative<std::vector<A1>, R, A1, A2, As...>::FuncA2B& a2b,
                    const typename Applicative<std::vector<A1>, R, A1, A2, As...>::FunctorA& fa)
    {
      typename Applicative<std::vector<A1>, R, A1, A2, As...>::FunctorB fb;
      for (auto& oldVal: fa)
        {
          auto newVal = a2b(oldVal);
          fb.push_back(newVal);
        }
      return std::move(fb);
    }
  };
  */

} // end namespace FuncA2BalCpp

#endif // VECTOR_HPP
