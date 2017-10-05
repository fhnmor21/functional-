
// Ref: http://yapb-soc.blogspot.com.au/2012/11/arrows-and-kleisli-in-c.html

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <utility>

/* MakeT T : X -> T X */
template < template < class... > class T >
struct MakeT
{
  template < class... X, class R = T< typename std::decay< X >::type... > >
  constexpr R operator()(X&&... x)
  {
    return R(std::forward< X >(x)...);
  }
};

/* pair : X x Y -> (X,Y) */
constexpr auto pair = MakeT< std::pair >();

struct sequence_tag
{
};
struct pointer_tag
{
};

template < class X >
X category(...);

template < class S >
auto category(const S& s) -> decltype(std::begin(s), sequence_tag());

template < class T >
struct Category
{
  using type = decltype(category< T >(std::declval< T >()));
};

template < class R, class... X >
struct Category< R (&)(X...) >
{
  using type = R (&)(X...);
};

template < class R, class... X >
struct Category< R (*)(X...) >
{
  using type = R (&)(X...);
};

template < class T >
using Cat = typename Category< typename std::decay< T >::type >::type;

/* closet : (A -> ... -> C) x A -> (... -> C) */
template < class F, class X >
struct Part
{
  F f = F();
  X x;

  template < class _F, class _X >
  constexpr Part(_F&& f, _X&& x)
      : f(std::forward< _F >(f))
      , x(std::forward< _X >(x))
  {
  }

  template < class _X >
  constexpr Part(_X&& x)
      : x(std::forward< _X >(x))
  {
  }

  template < class... Xs >
  constexpr auto operator()(Xs&&... xs) -> decltype(f(x, std::declval< Xs >()...))
  {
    return f(x, std::forward< Xs >(xs)...);
  }
};

template < class F, class X >
struct RPart
{
  F f = F();
  X x;

  template < class _F, class _X >
  constexpr RPart(_F&& f, _X&& x)
      : f(std::forward< _F >(f))
      , x(std::forward< _X >(x))
  {
  }

  template < class _X >
  constexpr RPart(_X&& x)
      : x(std::forward< _X >(x))
  {
  }

  template < class... Y >
  constexpr decltype(f(std::declval< Y >()..., x)) operator()(Y&&... y)
  {
    return f(std::forward< Y >(y)..., x);
  }
};

constexpr auto closet = MakeT< Part >();
constexpr auto rcloset = MakeT< RPart >();

/* compose : (B -> C) x (A -> B) -> (A -> C) */
template < class F, class G >
struct Composition
{
  F f;
  G g;

  template < class _F, class _G >
  constexpr Composition(_F&& f, _G&& g)
      : f(std::forward< _F >(f))
      , g(std::forward< _G >(g))
  {
  }

  template < class X >
  constexpr auto operator()(X&& x) -> decltype(f(g(std::declval< X >())))
  {
    return f(g(std::forward< X >(x)));
  }
};

constexpr auto compose = MakeT< Composition >();

template < class... >
struct Monad;

/* mbind : (A -> M<B>) x M<A> -> M<B> */
template < class F, class M, class... N, class Mo = Monad< Cat< M > > >
constexpr auto mbind(F&& f, M&& m, N&&... n)
    -> decltype(Mo::mbind(std::declval< F >(), std::declval< M >(), std::declval< N >()...))
{
  return Mo::mbind(std::forward< F >(f), std::forward< M >(m), std::forward< N >(n)...);
}

/* mdo : M<A> x M<B> -> M<B> */
template < class F, class M, class... N, class Mo = Monad< Cat< M > > >
constexpr auto mdo(F&& f, M&& m) -> decltype(Mo::mdo(std::declval< F >(), std::declval< M >()))
{
  return Mo::mdo(std::forward< F >(f), std::forward< M >(m));
}

// The first template argument must be explicit!
/* mreturn M : X -> M<X> */
template < class M, class X, class... Y, class Mo = Monad< Cat< M > > >
constexpr auto mreturn(X&& x, Y&&... y)
    -> decltype(Mo::template mreturn< M >(std::declval< X >(), std::declval< Y >()...))
{
  return Mo::template mreturn< M >(std::forward< X >(x), std::forward< Y >(y)...);
}

template < template < class... > class M,
           class X,
           class... Y,
           class _M = M< typename std::decay< X >::type >,
           class Mo = Monad< Cat< _M > > >
constexpr auto mreturn(X&& x, Y&&... y)
    -> decltype(Mo::template mreturn< _M >(std::declval< X >(), std::declval< Y >()...))
{
  return Mo::template mreturn< _M >(std::forward< X >(x), std::forward< Y >(y)...);
}

// Also has explicit template argument.
template < class M, class Mo = Monad< Cat< M > > >
auto mfail() -> decltype(Mo::template mfail< M >())
{
  return Mo::template mfail< M >();
}

template < class M, class F >
constexpr auto operator>>=(M&& m, F&& f)
    -> decltype(mbind(std::declval< F >(), std::declval< M >()))
{
  return mbind(std::forward< F >(f), std::forward< M >(m));
}

template < class M, class F >
constexpr auto operator>>(M&& m, F&& f) -> decltype(mdo(std::declval< M >(), std::declval< F >()))
{
  return mdo(std::forward< M >(m), std::forward< F >(f));
}

template < class F, class M >
constexpr auto operator^(F&& f, M&& m) -> decltype(fmap(std::declval< F >(), std::declval< M >()))
{
  return fmap(std::forward< F >(f), std::forward< M >(m));
}

template < class... >
struct Rebind;

template < template < class... > class S, class X >
struct Rebind< S< X > >
{
  template < class Y >
  using rebind = S< Y >;
};

template < class S, class X >
using rebind = typename Rebind< S >::template rebind< X >;

template <>
struct Monad< sequence_tag >
{

  template < class S >
  using mvalue = typename S::value_type;

  template < class F,
             template < class... > class S,
             class X,
             class R = typename std::result_of< F(X) >::type >
  static R mbind(F&& f, const S< X >& xs)
  {
    R r;
    for(const X& x : xs)
    {
      auto ys = std::forward< F >(f)(x);
      std::move(std::begin(ys), std::end(ys), std::back_inserter(r));
    }
    return r;
  }

  template < class F,
             template < class... > class S,
             class X,
             class Y,
             class R = typename std::result_of< F(X, Y) >::type >
  static R mbind(F&& f, const S< X >& xs, const S< Y >& ys)
  {
    R r;
    for(const X& x : xs)
    {
      for(const Y& y : ys)
      {
        auto zs = std::forward< F >(f)(x, y);
        std::move(std::begin(zs), std::end(zs), std::back_inserter(r));
      }
    }
    return r;
  }

  template < template < class... > class S, class X, class Y >
  static S< Y > mdo(const S< X >& mx, const S< Y >& my)
  {
    // Note: This is not a strictly correct definition.
    // It should return my concatenated to itself for every element of mx.
    return mx.size() ? my : S< Y >{};
  }

  template < class S, class X >
  static rebind< S, X > mreturn(X&& x)
  {
    return {std::forward< X >(x)}; // Construct an S of one element.
  }

  template < class S >
  static S mfail()
  {
    return S{};
  }
};

template <>
struct Monad< pointer_tag >
{

  template < class P >
  using mvalue = typename P::element_type;

  template < class F,
             template < class... > class Ptr,
             class X,
             class R = typename std::result_of< F(X) >::type >
  static R mbind(F&& f, const Ptr< X >& p)
  {
    return p ? std::forward< F >(f)(*p) : nullptr;
  }

  template < class F,
             template < class... > class Ptr,
             class X,
             class Y,
             class R = typename std::result_of< F(X, Y) >::type >
  static R mbind(F&& f, const Ptr< X >& p, const Ptr< Y >& q)
  {
    return p and q ? std::forward< F >(f)(*p, *q) : nullptr;
  }

  template < template < class... > class M, class X, class Y >
  static M< Y > mdo(const M< X >& mx, const M< Y >& my)
  {
    return mx ? (my ? mreturn< M< Y > >(*my) : nullptr) : nullptr;
  }

  template < class M, class X >
  static M mreturn(X&& x)
  {
    using Y = typename M::element_type;
    return M(new Y(std::forward< X >(x)));
  }

  template < class M >
  static M mfail()
  {
    return nullptr;
  }
};

constexpr bool is_int(char c) { return c >= '0' and c <= '9'; }

template < class... >
struct MonadZero;
template < class... >
struct MonadPlus;

/* mzero M : () -> M */
template < class M, class Mo = MonadZero< Cat< M > > >
auto mzero() -> decltype(Mo::template mzero< M >())
{
  return Mo::template mzero< M >();
}

/* mplus : M<A> x M<A> -> M<A> */
template < class A, class B, class Mo = MonadPlus< Cat< A > > >
auto mplus(A&& a, B&& b) -> decltype(Mo::mplus(std::declval< A >(), std::declval< B >()))
{
  return Mo::mplus(std::forward< A >(a), std::forward< B >(b));
}

template <>
struct MonadZero< sequence_tag >
{
  template < class S >
  S mzero()
  {
    return S{};
  }
};

template < class X, class Y >
auto operator+(X&& x, Y&& y) -> decltype(mplus(std::declval< X >(), std::declval< Y >()))
{
  return mplus(std::forward< X >(x), std::forward< Y >(y));
}

/* mplus( xs, ys ) = "append xs with ys" */
template <>
struct MonadPlus< sequence_tag >
{
  template < class A, class B >
  static A mplus(A a, const B& b)
  {
    std::copy(b.begin(), b.end(), std::back_inserter(a));
    return a;
  }
};

/* id : X -> X */
constexpr struct Id
{
  template < class X >
  constexpr X operator()(X&& x)
  {
    return std::forward< X >(x);
  }
} id{};

template < class... >
struct Arrow;

/* arr : (A -> B) -> Arrow A B */
template < class A, class F, class Arr = Arrow< Cat< A > > >
constexpr auto arr(F&& f) -> decltype(Arr::arr(std::declval< F >()))
{
  return Arr::arr(std::forward< F >(f));
}

template < class A >
struct Arr
{
  template < class F >
  constexpr auto operator()(F&& f) -> decltype(arr(std::declval< F >()))
  {
    return arr(std::forward< F >(f));
  }
};

/* split : (A -> B) x (X -> Y) -> (pair<A,X> -> pair<B,Y>) */
constexpr struct Split
{
  template < class F, class G, class A = Arrow< Cat< F > > >
  constexpr auto operator()(F&& f, G&& g)
      -> decltype(A::split(std::declval< F >(), std::declval< G >()))
  {
    return A::split(std::forward< F >(f), std::forward< G >(g));
  }
} split{};

/* fan : (A -> B) x (A -> C) -> (A -> pair<B,C>) */
constexpr struct Fan
{
  template < class F, class G, class A = Arrow< Cat< F > > >
  constexpr auto operator()(F&& f, G&& g)
      -> decltype(A::fan(std::declval< F >(), std::declval< G >()))
  {
    return A::fan(std::forward< F >(f), std::forward< G >(g));
  }
} fan{};

/* first : (A -> B) -> (pair<A,X> -> pair<B,X>) */
constexpr struct First
{
  template < class F, class A = Arrow< Cat< F > > >
  constexpr auto operator()(F&& f) -> decltype(A::first(std::declval< F >()))
  {
    return A::first(std::forward< F >(f));
  }
} first{};

/* first : (A -> B) -> (pair<X,A> -> pair<X,B>) */
constexpr struct Second
{
  template < class F, class A = Arrow< Cat< F > > >
  constexpr auto operator()(F&& f) -> decltype(A::second(std::declval< F >()))
  {
    return A::second(std::forward< F >(f));
  }
} second{};

/* duplicate : X -> (X,X) */
constexpr struct Duplicate
{
  template < class X, class P = std::pair< X, X > >
  constexpr P operator()(const X& x)
  {
    return P(x, x);
  }
} duplicate{};

template < class Binary >
struct Uncurrier
{
  Binary b;

  template < class P >
  constexpr auto operator()(P&& p)
      -> decltype(b(std::get< 0 >(std::declval< P >()), std::get< 1 >(std::declval< P >())))
  {
    return b(std::get< 0 >(std::forward< P >(p)), std::get< 1 >(std::forward< P >(p)));
  }
};

constexpr auto uncurry = MakeT< Uncurrier >();

/* pairCompose : (A -> B) x (X -> Y) -> (pair<A,X> -> pair<B,Y>) */
template < class F, class G >
struct PairComposition
{
  F f;
  G g;

  template < class _F, class _G >
  constexpr PairComposition(_F&& f, _G&& g)
      : f(std::forward< _F >(f))
      , g(std::forward< _G >(g))
  {
  }

  template < class P /*air*/ >
  constexpr auto operator()(const P& p)
      -> decltype(std::make_pair(f(std::get< 0 >(p)), g(std::get< 1 >(p))))
  {
    return std::make_pair(f(std::get< 0 >(p)), g(std::get< 1 >(p)));
  }
};

constexpr auto pairCompose = MakeT< PairComposition >();

template < class Func >
struct Arrow< Func >
{
  template < class F >
  static constexpr F arr(F&& f)
  {
    return std::forward< F >(f);
  }

  /* split(f,g)(x,y) = { f(x), g(y) } */
  template < class F, class G >
  static constexpr auto split(F f, G g) -> PairComposition< F, G >
  {
    return pairCompose(std::move(f), std::move(g));
  }

  /*
   * first(f)(x,y)  = { f(x), y }
   * second(f)(x,y) = { x, f(y) }
   */

  template < class F >
  static constexpr auto first(F f) -> decltype(split(std::move(f), id))
  {
    return split(std::move(f), id);
  }

  template < class F >
  static constexpr auto second(F f) -> decltype(split(id, std::move(f)))
  {
    return split(id, std::move(f));
  }

  /* fan(f,g)(x) = { f(x), g(x) } */
  template < class F, class G >
  static constexpr auto fan(F f, G g)
      -> decltype(compose(split(std::move(f), std::move(g)), duplicate))
  {
    return compose(split(std::move(f), std::move(g)), duplicate);
  }
};

/* Forwarder F : F */
template < class F >
struct Forwarder
{
  using function = typename std::decay< F >::type;
  function f = F();

  template < class... G >
  constexpr Forwarder(G&&... g)
      : f(std::forward< G >(g)...)
  {
  }

  template < class... X >
  constexpr auto operator()(X&&... x) -> decltype(f(std::declval< X >()...))
  {
    return f(std::forward< X >(x)...);
  }

  constexpr operator function() { return f; }
};

/* Kleisli M F : F -- where F : A -> M<B> */
template < template < class... > class M, class F = Id >
struct Kleisli : Forwarder< F >
{
  template < class... G >
  constexpr Kleisli(G&&... g)
      : Forwarder< F >(std::forward< G >(g)...)
  {
  }
};

template < template < class... > class M, class F, class K = Kleisli< M, F > >
constexpr K kleisli(F f)
{
  return K(std::move(f));
}

/* Composition : Kleisli(B -> M<C>) x Kleisli(A -> M<B>) -> (A -> M<C>) */
template < template < class... > class M, class F, class G >
struct Composition< Kleisli< M, F >, Kleisli< M, G > >
{
  Kleisli< M, F > f;
  Kleisli< M, G > g;

  template < class _F, class _G >
  constexpr Composition(_F&& f, _G&& g)
      : f(std::forward< _F >(f))
      , g(std::forward< _G >(g))
  {
  }

  template < class X >
  constexpr auto operator()(X&& x) -> decltype(g(std::forward< X >(x)) >>= f)
  {
    return g(std::forward< X >(x)) >>= f;
  }
};

/* Composition : Kleisli(B -> M<C>) x Kleisli(A -> M<B>) -> Kleisli(A -> M<C>)
 */
constexpr struct KCompose
{
  template < template < class... > class M, class F, class G >
  constexpr auto operator()(Kleisli< M, F > f, Kleisli< M, G > g)
      -> Kleisli< M, Composition< Kleisli< M, F >, Kleisli< M, G > > >
  {
    return kleisli< M >(compose(std::move(f), std::move(g)));
  }
} kcompose{};

template < class M >
struct Return
{
  template < class X >
  constexpr auto operator()(X&& x) -> decltype(mreturn< M >(std::declval< X >()))
  {
    return mreturn< M >(std::forward< X >(x));
  }
};

/*
 * liftM : (A -> B) x M<A> -> M<B>
 * liftM : (A x B -> C) x M<A> x M<B> -> M<C>
 */
constexpr struct LiftM
{
  template < class F, class M, class R = Return< typename std::decay< M >::type > >
  constexpr auto operator()(F&& f, M&& m)
      -> decltype(std::declval< M >() >>= compose(R(), std::declval< F >()))
  {
    return std::forward< M >(m) >>= compose(R(), std::forward< F >(f));
  }

  template < class F, class A, class B >
  constexpr auto operator()(F&& f, A&& a, B&& b)
      -> decltype(std::declval< A >() >>= compose(rcloset(LiftM(), std::declval< B >()),
                                                  closet(closet, std::declval< F >())))
  {
    return std::forward< A >(a) >>=
           compose(rcloset(LiftM(), std::forward< B >(b)), closet(closet, std::forward< F >(f)));
  }
} liftM{};

/* kleisliSplit : Kleisli(A -> M<B>) x Kleisli(X -> M<Y>) -> (piar<A,X> ->
 * M<pair<B,Y>>) */
template < template < class... > class M, class F, class G >
struct KleisliSplit
{
  F f;
  G g;

  constexpr KleisliSplit(F f, G g)
      : f(std::move(f))
      , g(std::move(g))
  {
  }

  template < class X, class Y >
  constexpr auto operator()(const std::pair< X, Y >& p)
      -> decltype(liftM(pair, f(std::get< 0 >(p)), g(std::get< 1 >(p))))
  {
    return liftM(pair, f(std::get< 0 >(p)), g(std::get< 1 >(p)));
  }
};

template < template < class... > class M, class F >
struct Arrow< Kleisli< M, F > >
{

  template < class G >
  using K = Kleisli< M, G >;

  template < class G >
  static constexpr auto arr(G g) -> Kleisli< M, Part< LiftM, G > >
  {
    return kleisli< M >(closet(liftM, std::move(g)));
  }

  template < class G >
  static constexpr auto first(G g) -> K< decltype(::split(std::move(g), arr(id))) >
  {
    // id is not a Kleisli.
    // The call to arr refers to the arr above, not ::arr.
    // arr(id) : Kleisli(X -> M X)
    return ::split(std::move(g), arr(id));
  }

  template < class G >
  static constexpr auto second(G g) -> K< decltype(::split(arr(id), std::move(g))) >
  {
    return ::split(arr(id), std::move(g));
  }

  template < class G >
  static constexpr auto split(Kleisli< M, F > f, Kleisli< M, G > g) -> K< KleisliSplit< M, F, G > >
  {
    return KleisliSplit< M, F, G >(std::move(f.f), std::move(g.f));
  }

  template < class G >
  static constexpr auto fan(Kleisli< M, F > f, Kleisli< M, G > g)
      -> K< decltype(compose(std::declval< KleisliSplit< M, F, G > >(), duplicate)) >
  {
    return compose(KleisliSplit< M, F, G >(std::move(f.f), std::move(g.f)), duplicate);
  }
};

static std::ostringstream oss;
template < class X >
static std::string show(const X& x)
{
  oss.str("");
  oss << x;
  return oss.str();
}

static std::string show(std::string str) { return str; }

static constexpr const char* show(const char* str) { return str; }

template < class X, class Y, class... Z >
static std::string show(const X& x, const Y& y, const Z&... z)
{
  return show(x) + show(y, z...);
}

std::ostream& operator<<(std::ostream& os, const std::string& s)
{
  return os << '"' << s.c_str() << '"';
}

template < class X, class Y >
std::ostream& operator<<(std::ostream& os, const std::pair< X, Y >& p)
{
  return os << '(' << p.first << ',' << p.second << ')';
}

template < class X >
std::ostream& operator<<(std::ostream& os, const std::unique_ptr< X >& p)
{
  if(p)
    os << "Just " << *p;
  else
    os << "Nothing";
  return os;
}

template < class X >
std::ostream& operator<<(std::ostream& os, const std::vector< X >& v)
{
  os << '[';

  for(auto it = std::begin(v); it != std::end(v); it++)
  {
    os << *it;
    if(std::next(it) != std::end(v))
      os << ',';
  }

  os << ']';
  return os;
}

namespace std
{
  std::string to_string(const std::string& s);
  template < class X, class Y >
  std::string to_string(const std::pair< X, Y >& p);

  std::string to_string(const std::string& s) { return "\"" + s + "\""; }

  template < class X, class Y >
  std::string to_string(const std::pair< X, Y >& p)
  {
    return "(" + to_string(p.first) + "," + to_string(p.second) + ")";
  }
}

/* string : X -> std::string */
constexpr struct ToString
{
  template < class X >
  std::string operator()(const X& x) const
  {
    return std::to_string(x);
  }
} string{};

constexpr struct MakeJust
{
  template < class X, class D = typename std::decay< X >::type, class P = std::unique_ptr< D > >
  P operator()(X&& x)
  {
    return P(new D(std::forward< X >(x)));
  }
} Just{};

constexpr auto JustK = kleisli< std::unique_ptr >(Just);

/*
 * Get 0 : (X,Y) -> X
 * Get 1 : (X,Y) -> Y
 */
template < size_t N >
struct Get
{
  template < class P >
  constexpr auto operator()(P&& p) -> decltype(std::get< N >(std::declval< P >()))
  {
    return std::get< N >(std::forward< P >(p));
  }
};

/* fcomp : (A -> B) x (B -> C) -> (A -> C) */
constexpr struct FComp
{
  template < class G, class F, class C = Composition< F, G > >
  constexpr C operator()(G g, F f)
  {
    return C(std::move(f), std::move(g));
  }
} fcomp{};

/* prefcomp : (A -> B) x Arrow B C -> Arrow A C */
constexpr struct PreFComp
{
  template < class F, class A >
  constexpr auto operator()(F&& f, A&& a) -> decltype(arr< A >(declval< F >()) > declval< A >())
  {
    return arr< A >(forward< F >(f)) > forward< A >(a);
  }
} prefcomp{};

/* prefcomp : Arrow A B x (B -> C) -> Arrow A C */
constexpr struct PostFComp
{
  template < class A, class F >
  constexpr auto operator()(A&& a, F&& f) -> decltype(declval< A >() > arr< A >(declval< F >()))
  {
    return forward< A >(a) > arr< A >(forward< F >(f));
  }
} postfcomp{};

int main()
{
  using std::cin;
  using std::cout;
  using std::endl;
  using std::to_string;

  auto plus2 = [](int x) { return x + 2; };

  std::pair< int, int > p(1, 1);

  cout << "(+2) *** (+2) >>> string *** (+2) $ (1,1) = "
       << compose(pairCompose(string, plus2), pairCompose(plus2, plus2))(p) << endl;

  cout << "(+2) *** (+2) >>> string *** (+2) $ 1 = "
       << compose(split(string, plus2), fan(plus2, plus2))(1) << endl;

  constexpr auto fst = Get< 0 >();
  constexpr auto snd = Get< 1 >();
  constexpr auto oneHundred = [](int x) { return 100; };

  // Hide the hundred.
  auto hidden = fan(fan(id, fan(oneHundred, id)), id)(0);
  // Function to find it again.
  auto find = fcomp(fcomp(fst, snd), fst);

  cout << "I found " << find(hidden) << "!" << endl;

  auto k1 = kleisli< std::vector >([](int x) -> std::vector< int > { return {x, x + 1}; });

  cout << "let k1 x = [x,x+1]\n";
  cout << "k1( k1(1) ) = " << compose(k1, k1)(1) << endl;

  auto stars = kleisli< std::vector >([](char c) {
    return c == '*' ? std::vector< char >{'+', '+'}
                    : c == '+' ? std::vector< char >{'*', ' ', '*'} : std::vector< char >{c};
  });

  auto starsSqr = kcompose(stars, stars);

  auto starsCube = kcompose(starsSqr, stars);

  cout << "stars of '*' : " << stars('*') << endl;
  cout << "stars^2 of '*' : " << starsSqr('*') << endl;
  cout << "stars^3 of '*' : " << starsCube('*') << endl;

  auto hairs = kleisli< std::vector >([](char c) -> std::vector< char > {
    return c == '*'
               ? std::vector< char >{'\'', '"', '\''}
               : c == '+' ? std::vector< char >{'"', ' ', '"'}
                          : c == '"' ? std::vector< char >{'\''}
                                     : c == '\'' ? std::vector< char >{} : std::vector< char >{c};
  });

  cout << "hairs of '*' : " << hairs('*') << endl;
  cout << "hairs^2 of '*' : " << compose(hairs, hairs)('*') << endl;

  cout << "split(stars,hairs) (*,*) = " << split(stars, hairs)(pair('*', '*')) << endl;
  cout << "fan(stars,hairs)    (*)  = " << fan(stars, hairs)('*') << endl;
  cout << "fan(hairs,stars)    (*)  = " << fan(hairs, stars)('*') << endl;
  cout << "split(hairs,stars) . fan(stars,hairs) = "
       << compose(split(hairs, stars), fan(stars, hairs))('*') << endl;

  // cout << "'*' => (stars,hairs) -> (hairs,stars) => "
  //     << kcompose( split(hairs,stars), fan(stars,hairs) )('*') << endl;

  cout << to_string(p) << " --- first(string) . second(plus2) ---> "
       << compose(first(string), second(plus2))(p) << endl;
  cout << to_string(p) << " --- first(string) . second(plus2) ---> " << split(string, plus2)(p)
       << endl;
}

// ====================================================
// Ref: http://yapb-soc.blogspot.com.au/2012/11/generic-function-objects-or.html
#include <utility>

using std::forward;
using std::declval;
using std::move;

template < class F, class X >
struct Part
{
  F f;
  X x;

  template < class _F, class _X >
  constexpr Part(_F&& f, _X&& x)
      : f(forward< _F >(f))
      , x(forward< _X >(x))
  {
  }

  template < class... Xs >
  constexpr auto operator()(Xs&&... xs) -> decltype(f(x, declval< Xs >()...))
  {
    return f(x, forward< Xs >(xs)...);
  }
};

template < class F, class X >
struct RPart
{
  F f;
  X x;

  template < class _F, class _X >
  constexpr RPart(_F&& f, _X&& x)
      : f(forward< _F >(f))
      , x(forward< _X >(x))
  {
  }

  template < class... Y >
  constexpr decltype(f(declval< Y >()..., x)) operator()(Y&&... y)
  {
    return f(forward< Y >(y)..., x);
  }
};

template < class F >
struct Binary
{
  template < class X >
  constexpr Part< F, X > operator()(X x)
  {
    return Part< F, X >(F(), move(x));
  }

  template < class X >
  constexpr RPart< F, X > with(X x)
  {
    return RPart< F, X >(F(), move(x));
  }
};

template < class F >
struct Chainable : Binary< F >
{
  using Binary< F >::operator();

  template < class X, class Y >
  using R = typename std::result_of< F(X, Y) >::type;

  // Three arguments: unroll.
  template < class X, class Y, class Z >
  constexpr auto operator()(X&& x, Y&& y, Z&& z) -> R< R< X, Y >, Z >
  {
    return F()(F()(std::forward< X >(x), std::forward< Y >(y)), std::forward< Z >(z));
  }

  template < class X, class Y, class... Z >
  using Unroll = typename std::result_of< Chainable< F >(typename std::result_of< F(X, Y) >::type,
                                                         Z...) >::type;

  // Any more? recurse.
  template < class X, class Y, class Z, class H, class... J >
  constexpr auto operator()(X&& x, Y&& y, Z&& z, H&& h, J&&... j) -> Unroll< X, Y, Z, H, J... >
  {
    // Notice how (*this) always gets applied at LEAST three arguments.
    return (*this)(F()(std::forward< X >(x), std::forward< Y >(y)),
                   std::forward< Z >(z),
                   std::forward< H >(h),
                   std::forward< J >(j)...);
  }
};

template < class F, class Fold >
struct Transitive : Binary< F >
{
  using Binary< F >::operator();

  template < class X, class Y, class Z >
  constexpr auto operator()(X&& x, Y&& y, Z&& z) -> typename std::result_of< F(X, Y) >::type
  {
    return Fold()(F()(forward< X >(x), forward< Y >(y)), F()(forward< Y >(y), forward< Z >(z)));
  }

  template < class X, class Y, class Z, class A, class... B >
  constexpr auto operator()(X&& x, Y&& y, Z&& z, A&& a, B&&... b) ->
      typename std::result_of< F(X, Y) >::type
  {
    return Fold()(F()(forward< X >(x), forward< Y >(y)),
                  F()(forward< Y >(y), forward< Z >(z), forward< A >(a), forward< B >(b)...));
  }
};

struct And : Chainable< And >
{
  using Chainable< And >::operator();

  template < class X, class Y >
  constexpr auto operator()(X&& x, Y&& y) -> decltype(declval< X >() && declval< Y >())
  {
    return forward< X >(x) && forward< Y >(y);
  }
};

constexpr struct Less : Transitive< Less, And >
{
  using Transitive< Less, And >::operator();

  template < class X, class Y >
  constexpr bool operator()(X&& x, Y&& y)
  {
    return forward< X >(x) < forward< Y >(y);
  }
} less{};

template < template < class... > class T >
struct ConstructT
{
  template < class... X, class R = T< typename std::decay< X >::type... > >
  constexpr R operator()(X&&... x)
  {
    return R(forward< X >(x)...);
  }
};

template < template < class... > class T >
struct ConstructChainable : Chainable< ConstructT< T > >
{
  using Self = ConstructChainable< T >;
  using Chainable< ConstructT< T > >::operator();

  template < class X >
  using D = typename std::decay< X >::type;
  template < class X, class Y, class R = T< D< X >, D< Y > > >
  constexpr R operator()(X&& x, Y&& y)
  {
    return R(forward< X >(x), forward< Y >(y));
  }
};
