#ifndef MAYBE_HPP
#define MAYBE_HPP

#include "either"

namespace FunctionalCpp
{
  struct Nothing
  {};

  template < class J >
  struct Just
  {
    Just(J j_) : value(j_) {};
    J value;
  };


  template < class V >
  struct Maybe
  {
    constexpr Maybe()
    : content_(Nothing())
      , Type(NOTHING)
    {}

    constexpr Maybe(V j_)
    : content_(Just<V>(j_))
      , Type(JUST)
    {}

    enum SubTypes
    {
      NOTHING = 0,
      JUST = 1,
    };

    const SubTypes Type;

 private:
    union UContent
    {
      // CTOR
      UContent(Nothing n_): n(n_) {};
      UContent(Just<V> j_): j(j_) {};

      // Data
      Nothing n;
      Just<V> j;
    };

    const UContent content_;

  };

  /*
  template < class J >
  struct Maybe
  {
    constexpr Maybe()
      : content_(Nothing())
      , Type(tNothing)
    {}

    constexpr Maybe(J j_)
    : content_(Just<J>(j_))
      , Type(tJust)
    {}

  private:
    enum InstanceType
    {
      tNothing,
      tJust,
    };

    union Content
    {
      // CTOR
      Content(Nothing n_): n(n_) {};
      Content(Just<J> j_): j(j_) {};

      // Data
      Nothing n;
      Just<J> j;
    };

    const Content content_;

    const InstanceType Type;
  };
  */

} // end namespace FunctionalCpp

#endif // MAYBE_HPP
