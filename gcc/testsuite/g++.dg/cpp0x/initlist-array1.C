// { dg-do compile { target c++11 } }

typedef int IRT[2];

const IRT& ir = IRT{1,2};
