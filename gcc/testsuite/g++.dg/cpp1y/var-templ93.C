// PR c++/125575
// CWG 1728, Type of an explicit instantiation of a variable template
// { dg-do compile { target c++14 } }

template<typename T> T var = T();
template float var<float>;
template int* var<int>;	  // { dg-error "type .int\\*. for explicit instantiation .var. does not match declared type .int." }
template auto var<char>;  // { dg-error "has no initializer" }
