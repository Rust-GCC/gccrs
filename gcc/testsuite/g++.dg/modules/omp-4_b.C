// PR c++/119864
// { dg-require-effective-target fopenmp }
// { dg-additional-options "-fmodules -fopenmp" }

import p1;

int main()
{
  T<1u> v[3u] = {};

  T s = sum(v, 3u);
}
