// PR c++/98524
// { dg-do compile { target c++14 } }

template<typename T> void (*f1)();
template void (*f1<int>)(int); // { dg-error "type .void \\(\\*\\)\\(int\\). for explicit instantiation .f1. does not match declared type .void \\(\\*\\)\\(\\)." }

template<typename T> void (*f2)(int) noexcept;
template void (*f2<int>)(int);  // { dg-error "type .void \\(\\*\\)\\(int\\). for explicit instantiation .f2. does not match declared type .void \\(\\*\\)\\(int\\) noexcept." "" { target c++17 } }

template<typename T> void (*f3)(int) ;
template void (*f3<int>)(int) noexcept; // { dg-error "type .void \\(\\*\\)\\(int\\) noexcept. for explicit instantiation .f3. does not match declared type .void \\(\\*\\)\\(int\\)." "" { target c++17 } }

template<typename T> void (*f4)(int) noexcept;
template void (*f4<int>)(int) noexcept;

template<typename T> void (*f5)(int);
template void (*f5<int>)(int);

template<typename T> void (*f6)(int);
template int (*f6<int>)(int); // { dg-error "type .int \\(\\*\\)\\(int\\). for explicit instantiation .f6. does not match declared type .void \\(\\*\\)\\(int\\)." }
