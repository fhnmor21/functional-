
#ifndef TYPE_UTILS_HPP
#define TYPE_UTILS_HPP

#include <type_traits>
#include <cstddef>

namespace FunctionalCpp
{
    ///
    // inner type of any parametric class
    template<typename T>
	struct inner_type { using type = T; };

    template<
            template<typename> class Tt,
            typename T>
    struct inner_type<Tt<T>> 
    {
		using type = T;
	};

	template<
			template<typename,typename...> class Tt,
			typename T,
			typename...Ts>
    struct inner_type<Tt<T,Ts...>> 
    {
        using type = T;
	};

    ///
    // rebind a context or container to another parameter
    template<
        class Cp,
        typename T>
    struct rebind_type
    {
        using type = Cp;
    };
    
    template<
            template<typename> class Cp,
            typename U,
            typename T>
    struct rebind_type<Cp<T>, U>
    {
        using type = Cp<U>;
    };

	template<
            template<typename...> class Cp,
            typename U,
			typename T,
			typename...Ts>
    struct rebind_type<Cp<T, Ts...>, U> 
    {
        using type = Cp<U, Ts...>;
	};

    ///
    // try to find the value type from containers
    template <typename C>
    struct value_type
    {
       typedef typename C::value_type type;
    };

    template <typename T, int N>
    struct value_type<T[N]>
    {
       typedef T type;
    };

    template <typename T>
    struct value_type<T*>
    {
      typedef T type;
    };
   

    ///
    // nth element from an template arguments pack
    template<std::size_t N, typename T, typename... Ts>
    struct get_type
    {
        using type = typename get_type<N - 1, Ts...>::type;
    };

    template<typename T, typename... Ts>
    struct get_type<0, T, Ts...>
    {
        using type = T;
    };



} // end namespace FunctionalCpp

#endif // TYPE_UTILS_HPP