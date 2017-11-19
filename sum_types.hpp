#ifndef SUM_TYPES_HPP
#define SUM_TYPES_HPP

namespace FunctionalCpp
{

  // acessor to sumtype values
namespace ST
{
  template < size_t I>
  struct get
  {};

  template<>
  struct get<0>
  {
    template < class S >
    static typename get_type_at<0, typename S::Types>::type
    value(const S* st_)
    {
      return st_->Content.s1;
    }
  };

  template<>
  struct get<1>
  {
    template < class S >
    static typename get_type_at<1, typename S::Types>::type
    value(const S* st_)
    {
      return st_->Content.s2;
    }
  };

  template<>
  struct get<2>
  {
    template < class S >
    static typename get_type_at<2, typename S::Types>::type
    value(const S* st_)
    {
      return st_->Content.s3;
    }
  };
}

  // S1 and S2 must be different types!
  template < class S1, class S2 >
  struct SumType
  {
    using Types = type_list<S1, S2>;
    //using Type1 = S1;
    //using Type2 = S2;
    //friend class ST::get<0>;
    //friend class ST::get<1>;

  //protected:
    // CTORs
    constexpr SumType(S1 s1_)
    : Content(s1_)
      , Type(static_cast<size_t>(0)) {}
    constexpr SumType(S2 s2_)
      : Content(s2_)
      , Type(static_cast<size_t>(1)) {}

    const size_t Type;

    union STContent
    {
      // CTORs
      constexpr STContent(S1 s1_): s1(s1_) {}
      constexpr STContent(S2 s2_): s2(s2_) {}

      // Data
      S1 s1;
      S2 s2;
    };

    const STContent Content;
  };


} // end namespace FunctionalCpp

#endif // SUM_TYPES_HPP
