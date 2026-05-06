// PR c++/123566
// { dg-do compile { target c++20 } }

template <class T, auto =
              [] {
                struct { // { dg-bogus "local class" "PR103901" { xfail *-*-* } }
                } s;	 // We would ICE trying to call the constructor.
              }>
bool v;
auto x = v<int>;
