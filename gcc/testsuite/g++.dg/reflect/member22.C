// PR c++/123726
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

struct S {
  template<typename>
  struct tt {};
  template<typename T>
  using tu = tt<T>;
};
int x;
void f() {}

template<typename>
struct tt {};

template<typename>
void tf() {}

template<typename>
int tv = 5;

template<typename T>
using tu = tt<T>;

int main() {
  S s;
  s.[:^^x:];		// { dg-error "not a member of .S." }
  s.[:^^s:];		// { dg-error "not a member of .S." }
  s.[:^^f:];		// { dg-error "not a member of .S." }
  s.[:^^tt:];		// { dg-error "invalid class member access of type template .tt." }
  s.template [:^^tf:];	// { dg-error "not a member of .S." }
  s.[:^^tv:];		// { dg-error "not a member of .S." }
  s.[:^^tu:];		// { dg-error "invalid class member access of type template .tu." }
  s.[:^^S::tt:];	// { dg-error "invalid class member access of type template .S::tt." }
  s.[:^^S::tu:];	// { dg-error "invalid class member access of type template .S::tu." }
}
