struct S {
  int (*a)(int) = [](int x) { return x * 2; };

  int b(int x, int (*f)(int) = [](int x) { return x * 3; }) {
    return f(x);
  }

  static int c(int x, int (*f)(int) = [](int x) { return x * 4; }) {
    return f(x);
  }
};

// this is an ODR violation to include this header in multiple TUs
// (the lambda is a different type in each TU, see [basic.def.odr] p15.6);
// keep the check that we can emit the lambda from a header unit,
// but skip it when checking ODR deduplication.
#ifndef CHECK_ODR_VIOLATIONS
inline int d(int x, int (*f)(int) = [](int x) { return x * 5; }) {
  return f(x);
}
#endif

// unevaluated lambdas
#if __cplusplus >= 202002L
struct E : decltype([](int x) { return x * 6; }) {
  decltype([](int x) { return x * 7; }) f;
};

template <typename T>
struct G : decltype([](int x) { return x * 8; }) {
  decltype([](int x) { return x * 9; }) h;
};

template <>
struct G<double> : decltype([](int x) { return x * 10; }) {
  decltype([](int x) { return x * 11; }) i;
};
#endif

// concepts
#if __cpp_concepts >= 201907L
template <typename T>
concept J = requires { []{ T(); }; };

template <typename T>
concept K = []{ return sizeof(T) == 1; }();
#endif

