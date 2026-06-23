// PR c++/125768
// { dg-additional-options -fmodules }

export module outer;
import api;
struct Outer {
  a_s sub;
  Outer();
};
