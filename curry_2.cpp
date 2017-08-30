#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include <iostream>
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

// closure
template<typename ArgVals, typename Funct>
using Context = std::pair< Funct, ArgVals  >;

// // prototypes
// template<int N, typename C, typename A1, typename... As >
// struct Closure_R;
// template<int N, typename C, typename A1 >
// struct Closure_1;

// //object function constructor
// template<int N, typename C, typename... As >
// auto applyClosure( C cntx )
// {
//     return Closure_R<N, C, As...>(cntx);
// }

// template<int N, typename C, typename A1 >
// auto applyClosure( C cntx )
// {
//     return Closure_1<N, C, A1>(cntx);
// }

// recutsive closures
template< int N, typename C, typename A1 >
struct Closure_1
{
    Closure_1(C cntx)
        : cntx_(cntx)
        , argsCount(std::tuple_size<typename std::remove_reference<typename C::second_type>::type>::value)
    {};
    
    auto operator()(A1 a1)
    {
        std::get<N>(cntx_.second) = a1;
        return apply_from_tuple(cntx_.first, cntx_.second);
    }

private: 
    C & cntx_;
    const std::size_t argsCount;
};


template< int N, typename C, typename A1, typename... As >
struct Closure_R
{
    Closure_R(C cntx)
        : cntx_(cntx)
        , argsCount(std::tuple_size<typename std::remove_reference<typename C::second_type>::type>::value)
    {};
    
    auto operator()(A1 a1)
    {
        if(N < argsCount)
        {
            std::get<N>(cntx_.second) = a1;
            Closure_R<N+1, C, As...> newClosure(cntx_);
            return newClosure;
            //return applyClosure<N+1, C, As...>( cntx_ );
        }
    }
    
private: 
    C & cntx_;
    const std::size_t argsCount;
};

template<typename R1, typename A1, typename... As>
struct Closure
{
    // constexpr size_t argsCount = sizeof...(as) + 2;
    using FunctIn   = std::function<R1(A1, As...)>;
    using Args      = std::tuple< A1, As... >;
    using Cntx      = Context< Args, FunctIn >;
    
    Closure(FunctIn fin)
        : cntx_(fin, Args())
    {};
    
    auto operator()(A1 a1)
    {
        std::get<0>(cntx_.second) = a1;
        Closure_R<1, Cntx, As...> newClosure(cntx_);
        return newClosure;
    }

private: 
    Context< Args, FunctIn > cntx_;
};


//object function constructor
template<typename R, typename A1, typename... As>
auto curry( R (*f) (A1, As...) )
{
    return Closure<R, A1, As...>(f);
}


// test function
int add2 (int a, int b)
{
    return a+b;
}

int add3 (int a, int b, int c)
{
    return a+b+c;
}

int add4 (int a, int b, int c, int d)
{
    return a+b+c+d;
}


int main()
{
    std::tuple<int, int> numbers2(1,2);
    std::cout <<  apply_from_tuple(add2, numbers2) << std::endl;
    std::tuple<int, int, int> numbers3(1,2,3);
    std::cout <<  apply_from_tuple(add3, numbers3) << std::endl;
    std::tuple<int, int, int, int> numbers4(1,2,3,4);
    std::cout <<  apply_from_tuple(add4, numbers4) << std::endl;
    
    // auto f4 = curry(add4);
    // std::cout << f4(40)(30)(20)(10) << std::endl;
    
}
