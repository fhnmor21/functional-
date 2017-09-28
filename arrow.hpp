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
  // struct Function : public std::function<B(A)>{};

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


} // end namespace FunctionalCpp

#endif // ARROW_HPP