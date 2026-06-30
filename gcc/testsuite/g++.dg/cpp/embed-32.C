// { dg-do preprocess }
// { dg-options "--embed-dir=${srcdir}/c-c++-common/cpp/embed-dir" }

#embed <magna-carta.txt> limit(1) offset (0) prefix(int a = ) suffix (;) // { dg-warning "'offset' parameter before C\\\+\\\+29 is a GCC extension" "" { target c++26_down } }
