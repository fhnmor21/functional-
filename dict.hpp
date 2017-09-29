#ifndef DICT_HPP
#define DICT_HPP

#include <map>
#include "functor.hpp"

namespace FunctionalCpp
{

  // ****************************************************************
  // local implementations / instatiations for STL containers

  /*
  struct Atom
  {
    std::string data;
    const bool operator <( const Atom& rhs ) const
    {
      return data < rhs.data;
    }
    const bool operator >( const Atom& rhs ) const
    {
      return data > rhs.data;
    }
  };
  */

  class Atom
  {
  public:
    enum class atomTypes
    {
      String,
      Integer,
      Number,
    };

    Atom() = delete;
    Atom(const Atom& other);
    explicit Atom(const std::string& s);
    explicit Atom(const char* c);
    // explicit Atom(long long i);
    explicit Atom(double d);

    const Atom& operator =( const Atom& rhs )
    {
      data_m = rhs.data_m;
      option_m = rhs.option_m;
      return *this;
    }

    const bool operator <( const Atom& rhs ) const
    {
      return data_m < rhs.data_m;
    }

    const bool operator >( const Atom& rhs ) const
    {
      return data_m > rhs.data_m;
    }

    const std::string& value() const { return data_m; }

  private:
    atomTypes option_m;
    std::string data_m;
  };

  Atom::Atom(const Atom &other)
    : option_m(other.option_m)
    , data_m(other.data_m) {}

  Atom::Atom(const std::string& s)
    : option_m(atomTypes::String)
    , data_m(s) {}

  Atom::Atom(const char* c)
    : option_m(atomTypes::String)
    , data_m(c) {}

  /*
  Atom::Atom(long long i)
    : option_m(atomTypes::Integer)
  {
    data_m = std::to_string(i);
  }
  */

  Atom::Atom(double d)
    : option_m(atomTypes::Number)
  {
    data_m = std::to_string(d);
  }


  template <class V>
  struct ADict
  {
    std::map<const Atom, V> data;
    const V& operator[](int k)
    {
      return data[Atom{static_cast<double>(k)}];
    }
    const V& operator[](double k)
    {
      return data[Atom{k}];
    }
    const V& operator[](const std::string& k)
    {
      return data[Atom{k}];
    }
    const V& operator[](const char* k)
    {
      return data[Atom{k}];
    }
  };

  // template <class V>
  // using ADict = std::map<Atom, V>;

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
