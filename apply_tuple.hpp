#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>

#ifndef APPLY_TUPLE
#define APPLY_TUPLE

namespace FunctionalCpp
{

namespace Tuple
{

  // tuple apply - SOURCE: http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html
  template<typename F, typename Tpl, size_t ...S >
  decltype(auto) apply_impl(F&& fn, Tpl&& t, std::index_sequence<S...>)
  {
    return std::forward<F>(fn)(std::get<S>(std::forward<Tpl>(t))...);
  }

  template<typename F, typename Tpl>
  decltype(auto) apply(F&& fn, Tpl&& t)
  {
	std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
	return apply_impl(std::forward<F>(fn),
		std::forward<Tpl>(t),
		std::make_index_sequence<tSize>());
  }


  template<typename F, typename Tpl, size_t ...S >
  decltype(auto) apply_ptr_impl(F&& fn, Tpl&& t, std::index_sequence<S...>)
  {
	return std::forward<F>(fn)(*(std::get<S>(std::forward<Tpl>(t)))...);
  }

  template<typename F, typename Tpl>
  decltype(auto) apply_ptr(F&& fn, Tpl&& t)
  {
	std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
	return apply_ptr_impl(std::forward<F>(fn),
			std::forward<Tpl>(t),
			std::make_index_sequence<tSize>());
  }

} // end namespace Tuple
} // end namespace FunctionalCpp

#endif
