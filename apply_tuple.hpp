#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>

// tuple apply - SOURCE: http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html
template<typename F, typename Tpl, size_t ...S >
decltype(auto) apply_from_tuple_impl(F&& fn, Tpl&& t, std::index_sequence<S...>)
{
	return std::forward<F>(fn)(std::get<S>(std::forward<Tpl>(t))...);
}

template<typename F, typename Tpl>
decltype(auto) apply_from_tuple(F&& fn, Tpl&& t)
{
	std::size_t constexpr tSize = std::tuple_size<typename std::remove_reference<Tpl>::type>::value;
	return apply_from_tuple_impl(std::forward<F>(fn),
	                             std::forward<Tpl>(t),
	                             std::make_index_sequence<tSize>());
}

