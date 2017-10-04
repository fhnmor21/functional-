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
  using Function = std::function<B(A)>;

  template <class A, class B, class C>
  using Function2 = std::function<C(A, B)>;

  // flip arguments
  template <class A, class B, class C>
  Function2<B, A, C> flip(Function2<A, B, C> ab2c)
  {
    Function2<B, A, C> ba2c = [ab2c] (B b, A a)
    {
      return ab2c(a, b);
    };
  }

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
    using type = Function<Arg, R>;
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
  // Category static class
  template < class A,
             class B,
             class C >
  struct Cat
  {
    static A id(A a)
    {
      return a;
    }

    static Function<A, C> compose( Function<A, B> a2b, Function<B, C> b2c)
    {
      Function<A, C> a2c = [a2b, b2c] (A a)
        {
          return b2c(a2b(a));
        };
      return std::move(a2c);
    }
  };

  template < class A >
  A id (A a)
  {
    return Cat<A, void, void>::id(a);
  }

  template < class A,
             class B,
             class C >
  Function<A, C> compose( Function<A, B> a2b, Function<B, C> b2c)
  {
    return Cat<A, B, C>::compose(a2b, b2c);
  }

  template < class A,
             class B,
             class C >
  Function<A, C> compose( B(a2b)(A), C(b2c)(B) )
  {
    Function<A, B> a2b_ = a2b;
    Function<B, C> b2c_ = b2c;
    Function<A, C> a2c_ = Cat<A, B, C>::compose( a2b_, b2c_ );
    return a2c_;
  }


  // ===
  // Arrow static class
  template < class A,
             class B,
             class C >
  struct Arrow : Cat<A, B, C>
  {
    //first :: (a `arr` b) -> ((a, c) `arr` (b, c))
    static std::tuple<B, C> first( Function<A, B> a2b,
                                   const std::tuple<A, C>& ac )
    {
      return std::move(std::make_tuple<B, C>(a2b(std::get<0>(ac)), std::get<1>(ac)));
    }
  };

  template < class A,
             class B,
             class C >
  std::tuple<B, C> first( Function<A, B> a2b,
                          const std::tuple<A, C>& ac )
  {
    return Arrow<A, B, C>::first(a2b, ac);
  }

  template < class A,
             class B,
             class C >
  std::tuple<A, C> second( Function<B, C> b2c,
                           const std::tuple<A, B>& ab)
  {
    std::tuple<B, A> ba = std::make_tuple<B, A>(std::get<1>(ab), std::get<0>(ab));
    std::tuple<C, A> ca = Arrow<A, B, C>::first(b2c, ba);
    std::tuple<A, C> ac = std::make_tuple<A, C>(std::get<1>(ca), std::get<0>(ca));
    return std::move(ac);

  }


} // end namespace FunctionalCpp

#endif // ARROW_HPP
