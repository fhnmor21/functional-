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

  namespace Vals
  {
    // tuple apply - SOURCE: http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html
    template<typename F, typename Tpl, size_t ...S >
    decltype(auto) invoke_impl(F&& fn, Tpl&& t, std::index_sequence<S...>)
    {
      return std::forward<F>(fn)(std::get<S>(std::forward<Tpl>(t))...);
    }

    template<typename F, typename Tpl>
    decltype(auto) invoke(F&& fn, Tpl&& t)
    {
    std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
    return invoke_impl(std::forward<F>(fn),
      std::forward<Tpl>(t),
      std::make_index_sequence<tSize>());
    }
  }

  namespace Ptrs
  {
    template<typename F, typename Tpl, size_t ...S >
    decltype(auto) invoke_impl(F&& fn, Tpl&& t, std::index_sequence<S...>)
    {
    return std::forward<F>(fn)(*(std::get<S>(std::forward<Tpl>(t)))...);
    }

    template<typename F, typename Tpl>
    decltype(auto) invoke(F&& fn, Tpl&& t)
    {
    std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
    return invoke_impl(std::forward<F>(fn),
        std::forward<Tpl>(t),
        std::make_index_sequence<tSize>());
    }
  }

} // end namespace Tuple
} // end namespace FunctionalCpp

#endif
