struct A {
  void f(int) const;
  static void f(const A*, int);

#if __cpp_explicit_this_parameter
  void g(this A*, int);
  static void g(A*, int);

  void h(int);
  void h(this A*, int);
#endif
};

template<class T>
struct B {
  void f(int) const;
  static void f(const B*, int);

#if __cpp_explicit_this_parameter
  void g(this B*, int);
  static void g(B*, int);

  void h(int);
  void h(this B*, int);
#endif
};

struct C {
  template<class T> void f(int) const;
  template<class T> static void f(const C*, int);

#if __cpp_explicit_this_parameter
  template<class T> void g(this C*, int);
  template<class T> static void g(C*, int);

  template<class T> void h(int);
  template<class T> void h(this C*, int);
#endif
};
