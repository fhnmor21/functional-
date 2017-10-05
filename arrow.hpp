#ifndef ARROW_HPP
#define ARROW_HPP

#include "type_utils.hpp"
#include "invoke.hpp"

/*
namespace std {
  template<class> class function; // undefined
  template<class R, class... ArgTypes>
  class function<R(ArgTypes...)> {
    public:
    using result_type = R;
    // construct/copy/destroy:
    function() noexcept;
    function(nullptr_t) noexcept;
    function(const function&);
    function(function&&);
    template<class F> function(F);
    function& operator=(const function&);
    function& operator=(function&&);
    function& operator=(nullptr_t) noexcept;
    template<class F> function& operator=(F&&);
    template<class F> function& operator=(reference_wrapper<F>) noexcept;
    ~function();
    // function modifiers:
    void swap(function&) noexcept;
    // function capacity:
    explicit operator bool() const noexcept;
    // function invocation:
    R operator()(ArgTypes...) const;
    // function target access:
    const type_info& target_type() const noexcept;
    template<class T> T* target() noexcept;
    template<class T> const T* target() const noexcept;
  };
  // Null pointer comparisons:
  template <class R, class... ArgTypes>
  bool operator==(const function<R(ArgTypes...)>&, nullptr_t) noexcept;
  template <class R, class... ArgTypes>
  bool operator==(nullptr_t, const function<R(ArgTypes...)>&) noexcept;
  template <class R, class... ArgTypes>
  bool operator!=(const function<R(ArgTypes...)>&, nullptr_t) noexcept;
  template <class R, class... ArgTypes>
  bool operator!=(nullptr_t, const function<R(ArgTypes...)>&) noexcept;
  // specialized algorithms:
  template <class R, class... ArgTypes>
  void swap(function<R(ArgTypes...)>&, function<R(ArgTypes...)>&);
}
*/

namespace FunctionalCpp
{
  template <class A, class B>
  using Function1 = std::function<B(A)>;

  template <class A, class B, class C>
  using Function2 = std::function<C(A, B)>;

  namespace Impl_
  {
    // **************************************************
    // recursive std::function return type for partial applications
    template < size_t N,
               class Ret,
               class... Args >
    struct Nested
    {};

    template < size_t N,
               class Ret,
               class Arg,
               class... Args >
    struct Nested< N, Ret, Arg, Args... > : Nested< N-1, Ret, Args... >
    {};

    template < class Ret,
               class Arg,
               class... Args >
    struct Nested< 0, Ret, Arg, Args... >
    {
    private:
    using R = typename Nested< 0, Ret, Args... >::type;
    public:
    using type = Function1<Arg, R>;
    };

    template < class Ret >
    struct Nested< 0, Ret >
    {
    public:
    using type = Ret;
    };
  } // end namespace Impl_

  template < class Ret,
             class... Args>
  using Curried = Impl_::Nested<0, Ret, Args...>;


  // ===
  // flip arguments
  template <class A, class B, class R>
  Function2<B, A, R> flip(Function2<A, B, R> ab2r)
  {
    Function2<B, A, R> ba2r = [ab2r] (B b, A a)
    {
      return ab2r(a, b);
    };
    return std::move(ba2r);
  }


  // ===
  // Id Function
  template < class A >
  A id(A a)
  {
    return a;
  }


  // ===
  // Category static class
  template < class A,
             class B,
             class C >
  struct Cat
  {
    // (.) :: (b `arr` c) -> (a `arr` b) -> (a `arr` c)
    static Function1<A, C> compose( Function1<B, C> b2c, Function1<A, B> a2b )
    {
      Function1<A, C> a2c = [b2c, a2b] (A a)
        {
          return b2c( a2b(a) );
        };
      return std::move(a2c);
    }
  };

  template < class A,
             class B,
             class C >
  // (.) :: (b `arr` c) -> (a `arr` b) -> (a `arr` c)
  Function1<A, C> compose( Function1<B, C> b2c, Function1<A, B> a2b )
  {
    return Cat<A, B, C>::compose(a2b, b2c);
  }

  template < class A,
             class B,
             class C >
  // (.) :: (b `arr` c) -> (a `arr` b) -> (a `arr` c)
  Function1<A, C> compose( C(b2c)(B), B(a2b)(A) )
  {
    Function1<A, B> a2b_ = a2b;
    Function1<B, C> b2c_ = b2c;
    Function1<A, C> a2c_ = Cat<A, B, C>::compose( b2c_, a2b_ );
    return a2c_;
  }


  // ===
  // Arrow static class
  template < class A,
             class B,
             class C,
             class D >
struct Arrow : Cat<A, B, C>
  {
    // (***) :: (a `arr` c) -> (b `arr` d) -> ((a, b) `arr` (c, d))
    static Function1< std::tuple<A, B>, std::tuple<C, D> > prod3( Function1<A, C> a2c, Function1<B, D> b2d )
    {
      Function1< std::tuple<A, B>, std::tuple<C, D> > ppp = [a2c, b2d](std::tuple<A, B> ab)
      {
        return std::move(   std::make_tuple<C, D>
                            (
                              a2c(std::get<0>(ab)),
                              b2d(std::get<1>(ab))
                            )
                        );
      };
      return ppp;
    }
  };

} // end namespace FunctionalCpp

#endif // ARROW_HPP
