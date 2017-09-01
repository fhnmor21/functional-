#include <iostream>
#include <tuple>
#include <functional>

template < typename Function , typename... Args >
auto curry( Function&& f , Args&&... args );


// binder
template< typename Function, typename... BoundArgs >
struct CurryingBinder
{
    CurryingBinder( Function const &f, BoundArgs&&... as )
    : f_(f)
    , args_(std::forward<BoundArgs>(as)...)
    {}

    template< typename... OtherArgs >
    auto operator()( OtherArgs&&... oas )
    {
        return call ( std::make_index_sequence< sizeof...(BoundArgs) >()
                    , std::forward< OtherArgs >(oas)... );
    }

private:
    // data
    Function const& f_;
    std::tuple< typename std::remove_reference<BoundArgs>::type... > args_;

    // call and dispathcer
    struct curry_arguments {};
    struct try_to_invoke_function : curry_arguments {};

    template< typename... Args >
    auto dispatch( try_to_invoke_function, Args&&... args ) const
      -> decltype( f_ (args...) )
    {
      return f_( std::forward<Args>(args)... );
    }

    template< typename... Args >
    auto dispatch( curry_arguments, Args&&... args ) const
      -> decltype( curry( f_, std::forward<Args>(args)... ) )
    {
        return curry( f_, std::forward<Args>(args)... );
    }

    template< std::size_t... Ns, typename... OtherArgs >
    auto call( std::index_sequence< Ns... >, OtherArgs&&... oas )
    {
        return dispatch( try_to_invoke_function{}, std::get< Ns >(args_)...
                       , std::forward< OtherArgs >(oas)... );
    }
};

// currying
template < typename Function, typename... Args>
auto curry (Function&& f, Args&&... args)
{
  return CurryingBinder<Function, Args...>( f, std::forward<Args>(args)... );
}

// test function
int add1 (double a)
{
  return a+1;
}

int add2 (int a, int b)
{
  return a+b;
}

int add3 (int a, int b, int c)
{
  return a+b+c;
}

int add4 (int a, float b, long c, double& d)
{
  return a+b+c+d;
}

int main()
{
  double  val = 1;
  const std::string msg("Hello Curry!\n");

//   auto f6 = curry(add6);
//   std::cout << f6(4)(3)(2)(val)(false)(msg) << std::endl;
//   auto f5 = curry(add5);
//   std::cout << f5(4)(3)(2)(val)(true) << std::endl;
  auto f4 = curry(add4);
  std::cout << f4(4)(3)(2)(val) << std::endl;
  auto f3 = curry(add3);
  std::cout << f3(4)(3)(2) << std::endl;
  auto f2 = curry(add2);
  std::cout << f2(4)(3) << std::endl;

  
}
