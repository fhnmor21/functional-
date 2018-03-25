#ifndef SUM_TYPES_HPP
#define SUM_TYPES_HPP

namespace FunctionalCpp
{

namespace ST
{
  // acessor to sumtype values
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

  // nested unions for sum type contents
  template <class S1, class... Sn>
  union NestedUnion
  {
    constexpr explicit NestedUnion (S1 s1_): head(s1_) {};
    S1 head;
    NestedUnion<Sn...> tail;
  };

  template <class S1, class S2>
  union NestedUnion<S1, S2>
  {
    constexpr explicit NestedUnion (S1 s1_): head(s1_) {};
    constexpr explicit NestedUnion (S2 s2_): tail(s2_) {};
    S1 head;
    S2 tail;
  };

  template <class T1, class T2, class... Tn>
  struct UnionType
  {
    using Types = type_list<T1, T2, Tn...>;
    NestedUnion<T1, T2, Tn...> Content;

    UnionType() = delete;
    //UnionType(UnionType &) = delete;
    //UnionType(UnionType &&) = delete;

    template <class S>
    constexpr UnionType(const S& val) : Content(val) {};
  };

  template <size_t I>
  struct UTGet
  {
    template <class UT>
    static typename get_type_at<I, typename UT::Types>::type
    value (const UT& ut)
    {
      return UTGet<I-1>::value((ut->Content.tail));
    }
  };

  template <>
  struct UTGet<1>
  {
    template <class UT>
    static typename get_type_at<1, typename UT::Types>::type
    value (const UT& ut)
    {
      return ut.Content.tail;
    }
  };

  template <>
  struct UTGet<0>
  {
    template <class UT>
    static typename get_type_at<0, typename UT::Types>::type
    value (const UT& ut)
    {
      return ut.Content.head;
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
