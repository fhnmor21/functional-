#ifndef DICT_HPP
#define DICT_HPP

#include <map>
#include "functor.hpp"

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  class Atom
  {
  public:
    enum class atomTypes
    {
      String,
      Integer,
      Number,
    };

    // Atom() = delete;
    // Atom(Atom&) = delete;    
    explicit Atom(const std::string& s);
    explicit Atom(const char* c);
    explicit Atom(long long i);
    explicit Atom(double d);

    const std::string& value() const { return data_m; }

    bool operator()( const Atom& lhs, const Atom& rhs ) const
    {
      return lhs.data_m < rhs.data_m;
    }

  private:
    atomTypes option_m;
    std::string data_m;
  };

  Atom::Atom(const std::string& s)
    : option_m(atomTypes::String)
    , data_m(s) {}

  Atom::Atom(const char* c)
    : option_m(atomTypes::String)
    , data_m(c) {}

  Atom::Atom(long long i)
    : option_m(atomTypes::Integer)
  {
    data_m = std::to_string(i);
  }

  Atom::Atom(double d)
    : option_m(atomTypes::Number)
  {
    data_m = std::to_string(d);
  }

  template <class V>
  struct ADict
  {
    std::map<const Atom, V> data;
  };



  // instance for std::map
  template <class V>
  using Dict = std::map<const std::string, V>; //, std::less<std::string>, std::allocator<std::pair<const std::string, V>>;

  template < class Ret,
             class Arg >
  struct Functor<Function<Arg, Ret>, Dict<Arg>, Dict<Ret>>
  {
    static Dict<Ret> fmap(const Function<Arg, Ret> a2b,
                          const Dict<Arg>& fa)
    {
      Dict<Ret> fb;

      for (auto& oldVal: fa)
      {
        auto newVal = a2b(oldVal);
        fb.push_back(newVal);
      }
      return std::move(fb);
    }
  };


  template < class Ret,
             class Arg >
  struct Applicative<Dict<Function<Arg, Ret>>, Dict<Arg>, Dict<Ret>>
  {
    static Dict<Arg> apply(const Dict<Function<Arg, Ret>>& fa2b,
                            const Dict<Arg>& fa)
    {
      Dict<Ret> fb;

      for(auto& a2b : fa2b)
      {
        for (auto& oldVal: fa)
        {
          auto newVal = a2b(oldVal);
          fb.push_back(newVal);
        }
      }
      return std::move(fb);
    }
  };

} // end namespace FuncA2BalCpp

#endif // DICT_HPP
