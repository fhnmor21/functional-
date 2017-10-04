#ifndef APPLY_TUPLE
#define APPLY_TUPLE

#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>

namespace FunctionalCpp
{

  namespace Tuple
  {

    template<class A1, class A2, class... As>
    struct Swapper
    {
    private:
      static std::tuple<A2, A1, As...>  swap_impl2(A1 a1, A2 a2, As... as)
      {
        return std::move(std::tuple< A2, A1, typename std::remove_reference<As>::type... >(a2, a1, as...));
      }

      template <size_t ...S >
      static std::tuple<A2, A1, As...> swap_impl1(std::tuple<A1, A2, As...> from, std::index_sequence<S...> )
      {
        return Swapper<A1, A2, As...>::swap_impl2(std::get<S>(std::forward< std::tuple<A1, A2, As...> >(from))...);
      }

    public:
      static std::tuple<A2, A1, As...>  swap(std::tuple<A1, A2, As...> from)
      {
        size_t constexpr tSize = std::tuple_size<typename std::remove_reference< std::tuple<A1, A2, As...> >::type>::value;
        return Swapper<A1, A2, As...>::swap_impl1( from, std::make_index_sequence<tSize>() );
      }
    };

    template<class A1, class A2, class... As>
    std::tuple<A2, A1, As...>  swap(std::tuple<A1, A2, As...> from)
    {
      return Swapper<A1, A2, As...>::swap(from);
    }

    namespace Vals
    {
      // tuple apply - SOURCE: http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html
      template<class F, class Tpl, size_t ...S >
      decltype(auto) invoke_impl(F&& fn, Tpl&& t, std::index_sequence<S...>)
      {
        return std::forward<F>(fn)(std::get<S>(std::forward<Tpl>(t))...);
      }

      template<class F, class Tpl>
      decltype(auto) invoke(F&& fn, Tpl&& t)
      {
      std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
      return invoke_impl(std::forward<F>(fn),
        std::forward<Tpl>(t),
        std::make_index_sequence<tSize>());
      }
    } // end namespace Vals

    namespace Ptrs
    {
      template<class F, class Tpl, size_t ...S >
      decltype(auto) invoke_impl(F&& fn, Tpl&& t, std::index_sequence<S...>)
      {
      return std::forward<F>(fn)(*(std::get<S>(std::forward<Tpl>(t)))...);
      }

      template<class F, class Tpl>
      decltype(auto) invoke(F&& fn, Tpl&& t)
      {
      std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
      return invoke_impl(std::forward<F>(fn),
          std::forward<Tpl>(t),
          std::make_index_sequence<tSize>());
      }
    }// end namespace Ptrs

  } // end namespace Tuple

} // end namespace FunctionalCpp

#endif
