// PR c++/125697
// { dg-do compile { target c++11 } }
// { dg-options "-Wrange-loop-construct" }

template <typename> struct pair {
  int first;
  int second;
  template <typename _U1> pair(const pair<_U1> &&);
};
void
foo (const pair<int> (&arr)[1])
{
  for (const auto x : arr)  // { dg-bogus "creates a copy" }
    (void) x;
}
static_assert(__is_trivially_copyable(pair<int>));
static_assert(!__is_trivially_constructible(pair<int>));

template <typename> struct pairbig {
  int first;
  int second;
  char arr[64];
  template <typename _U1> pairbig(const pairbig<_U1> &&);
};
void
bar (const pairbig<int> (&arr)[1])
{
  for (const auto x : arr)  // { dg-warning "creates a copy" }
    (void) x;
}
static_assert(__is_trivially_copyable(pairbig<int>));
static_assert(!__is_trivially_constructible(pairbig<int>));

template <typename> struct pairnontriv {
  int first;
  int second;
  pairnontriv(const pairnontriv &);
  template <typename _U1> pairnontriv(const pairnontriv<_U1> &&);
};
void
baz (const pairnontriv<int> (&arr)[1])
{
  for (const auto x : arr)  // { dg-warning "creates a copy" }
    (void) x;
}
static_assert(!__is_trivially_copyable(pairnontriv<int>));
static_assert(!__is_trivially_constructible(pairnontriv<int>));
