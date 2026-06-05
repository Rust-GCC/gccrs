// PR c++/125613
// { dg-do compile { target c++11 } }

template<typename T>
void fn1 (T) {}
template void fn1<int>(int); // OK

template<typename T>
void fn2 (T) noexcept {}
template void fn2<int>(int) noexcept; // OK

template<typename T>
void fn3 (T) {}
template void fn3<int>(int) noexcept; // { dg-error "exception specification .noexcept. in explicit instantiation does not match the instantiated one .noexcept \\(false\\)." }

template<typename T>
void fn4 (T) noexcept {}
template void fn4<int>(int); // OK

template<typename T, bool B>
void fn5 (T) noexcept (B) {}
template void fn5<int, true>(int) noexcept; // OK

template<typename T, bool B>
void fn6 (T) noexcept (B) {}
template void fn6<int, false>(int) noexcept; // { dg-error "exception specification .noexcept. in explicit instantiation does not match the instantiated one .noexcept \\(false\\)." }

template<typename T, bool B>
void fn7 (T) noexcept (B) {}
template void fn7<int, true>(int) noexcept(true); // OK

template<typename T, bool B>
void fn8 (T) noexcept (B) {}
template void fn8<int, false>(int) noexcept(false); // OK

template<typename T, bool B>
void fn9 (T) noexcept (B) {}
template void fn9<int, true>(int) noexcept(false); // { dg-error "exception specification .noexcept \\(false\\). in explicit instantiation does not match the instantiated one .noexcept." }

template<typename T>
void fn10 (T) noexcept {}
template void fn10<int>(int) noexcept(false); // { dg-error "exception specification .noexcept \\(false\\). in explicit instantiation does not match the instantiated one .noexcept." }

template<typename T>
struct S {
  void mfn1 () noexcept { }
  void mfn2 () { }
  template<bool B>
  void mfn3 () noexcept (B) { }
  template<bool B>
  void mfn4 () noexcept (B) { }
};

template void S<int>::mfn1 () noexcept;
template void S<int>::mfn2 () noexcept; // { dg-error "exception specification .noexcept. in explicit instantiation does not match the instantiated one .noexcept \\(false\\)." }
template void S<int>::mfn3<true>() noexcept;
template void S<int>::mfn4<false>() noexcept; // { dg-error "exception specification .noexcept. in explicit instantiation does not match the instantiated one .noexcept \\(false\\)." }

template<typename>
struct X {
  X() {}
  ~X() {}
};
template X<int>::X();
template X<int>::~X();

template<typename>
struct Y {
  Y() {}
  ~Y() {}
};
template Y<int>::Y() noexcept; // { dg-error "exception specification .noexcept. in explicit instantiation does not match the instantiated one .noexcept \\(false\\)." }
template Y<int>::~Y() noexcept;

template<typename>
struct Z {
  Z() noexcept {}
  ~Z() {}
};
template Z<int>::Z() noexcept;
template Z<int>::~Z() noexcept(false); // { dg-error "exception specification .noexcept \\(false\\). in explicit instantiation does not match the instantiated one .noexcept." }
