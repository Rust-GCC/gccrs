// PR c++/125575
// CWG 1704, Type checking in explicit instantiation of variable templates
// { dg-do compile { target c++14 } }
// Test from [temp.explicit].

template<typename T> T var = {};
template float var<float>;      // OK, instantiated variable has type float
template int var<int[16]>[];    // OK, absence of major array bound is permitted
template int *var<int>;         // error: instantiated variable has type int
// { dg-error "type .int\\*. for explicit instantiation .var. does not match declared type .int." "" { target *-*-* } .-1 }

template<typename T> auto av = T();
template int av<int>;           // OK, variable with type auto can be redeclared with type int

template<typename T> auto f() {}
template void f<int>();         // error: function with deduced return type
                                // redeclared with non-deduced return type ([dcl.spec.auto])
// { dg-error "does not match any template declaration" "" { target *-*-* } .-2 }
