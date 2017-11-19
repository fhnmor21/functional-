#ifndef EITHER_HPP
#define EITHER_HPP

#include "sum_types.hpp"

namespace FunctionalCpp
{

  template < class L, class R >
  struct Either: SumType<L, R>
  {
  public:
    // CTORs
    constexpr Either(L l_)
    : SumType<L, R>(l_)
    {};
    constexpr Either(R r_)
    : SumType<L, R>(r_)
    {};

  protected:
    // SubTypes
    constexpr static size_t LEFT = 0;
    constexpr static size_t RIGHT = 1;
  };

  template < class L, class R >
  struct Left : Either<L, R>
  {
    constexpr Left(L l_) : Either<L, R>(l_) {};
    const L operator()() const
    {
      return ST::get<0>::value(this);
    }
  };

  template < class L, class R >
  struct Right : Either<L, R>
  {
    constexpr Right(R r_) : Either<L, R>(r_) {};
    const R operator()() const
    {
      return ST::get<this->RIGHT>::value(this);
    }
  };

} // end namespace FunctionalCpp

#endif // EITHER_HPP
