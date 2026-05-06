// 13.1: ...cannot be overloaded if any of them, but not all, have a
// ref-qualifier.
// In C++20 this was relaxed by P1787R6 "Declarations and where to find them"

// { dg-require-effective-target c++11 }

class Y {
  void h() &;
  void h() const &;	       // OK
  void h() &&;		       // OK, all declarations have a ref-qualifier
  void i() &;		       // { dg-message "" "" { target c++17_down } }
  void i() const;	       // { dg-error "" "" { target c++17_down } }
			       // prior declaration of i has a ref-qualifier
};
