// PR c++/61745

template <typename INT,INT P> class Zp;

template <typename INT,INT P> 
Zp<INT,P> operator-(const Zp<INT,P>& a, const Zp<INT,P>& b);

template <typename INT,INT P>
class Zp {
public:
  static const INT p = P;
private:
  INT val;
public:
  Zp() : val(0) {}
  Zp( INT x ) : val(x%p) { if (x < 0 ) x+= p; }

  // this compiles only if the following definition is moved
  // AFTER the friend declaration
  Zp  operator-() const { return Zp(p-val); }
  // In C++2A, we have an unqualified-id (operator-) followed by
  // '<', and name lookup found a function.
  friend Zp<INT,P> operator- <>(const Zp<INT,P>& a, const Zp<INT,P>& b); // { dg-error "20:declaration of .operator\\-. as non-function" "" { target c++17_down } }
  // { dg-error "expected" "" { target c++17_down } .-1 }
};
