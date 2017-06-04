//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2016-2017.
//  Released under the MIT Software license; see doc/LICENSE
//
//  This class is a drop-in wrapper for std::array, adding on bounds checking.
//  If EMP_NDEBUG is set then it reverts back to std::array.


#ifndef EMP_ARRAY_H
#define EMP_ARRAY_H

#include <initializer_list>
#include <array>

#include "assert.h"
#include "../meta/TypeID.h"


#ifdef EMP_NDEBUG

namespace emp {
  template <typename T, size_t N> using array = std::array<T,N>;
}


#else // #EMP_NDEBUG *not* set

namespace emp {

  template <typename T, size_t N>
  class array {
  private:
    std::array<T, N> a;

  public:

    using value_type = T;

    array() : a() { ; }
    array(const emp::array<T,N> &) = default;
    array(std::initializer_list<T> in_list) : a(in_list) { ; }
    array(const std::array<T,N> & in) : a(in) { ; }         // Emergency fallback conversion.
    ~array() = default;

    constexpr uint32_t size() const noexcept { return N; }

    emp::array<T,N> & operator=(const emp::array<T,N> &) = default;

    operator std::array<T,N>() const {return a;};

    bool operator==(const emp::array<T,N> & in) const { return a == in.a; }
    bool operator!=(const emp::array<T,N> & in) const { return a != in.a; }
    bool operator<(const emp::array<T,N> & in)  const { return a < in.a; }
    bool operator<=(const emp::array<T,N> & in) const { return a <= in.a; }
    bool operator>(const emp::array<T,N> & in)  const { return a > in.a; }
    bool operator>=(const emp::array<T,N> & in) const { return a >= in.a; }

    T & operator[](size_t pos) {
      emp_assert(pos < (int) N, pos);
      return a[pos];
    }

    const T & operator[](size_t pos) const {
      emp_assert(pos < (int) N, pos);
      return a[pos];
    }

    auto begin() -> decltype(a.begin()) { return a.begin(); }
    auto end() -> decltype(a.end()) { return a.end(); }
    auto begin() const -> const decltype(a.begin()) { return a.begin(); }
    auto end() const -> const decltype(a.end()) { return a.end(); }

    T & back() { return a.back(); }
    const T & back() const { return a.back(); }

    template <typename... ARGS>
    auto fill(ARGS &&... args) -> decltype(a.fill(std::forward<ARGS>(args)...)) {
      return a.fill(std::forward<ARGS>(args)...);
    }
  };

}

#endif

namespace emp{
  // Ensure that emp::array works with TypeID
  template <typename T, size_t N> struct TypeID<emp::array<T,N>> {
    static std::string GetName() {
      std::stringstream ss;
      ss << "emp::array<" << TypeID<T>::GetName() << "," << N << ">";
      return ss.str();
    }
  };
}



#endif
