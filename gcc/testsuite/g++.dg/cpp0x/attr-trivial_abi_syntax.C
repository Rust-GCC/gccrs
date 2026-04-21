// { dg-do compile { target c++11 } }
// Test comprehensive trivial_abi attribute behavior
// Based on clang's attr-trivial-abi.cpp test

void __attribute__ ((trivial_abi))
foo (); // { dg-warning "'trivial_abi' attribute only applies to classes" }

// Should not crash.
template <class> class __attribute__ ((trivial_abi)) a
{
  a (a &&);
};

struct [[clang::trivial_abi]] S0
{
  int a;
};

struct __attribute__ ((trivial_abi)) S1
{
  int a;
};

struct __attribute__ ((trivial_abi))
S3 // { dg-warning "'trivial_abi' cannot be applied to 'S3'" }
{  // { dg-message "is polymorphic" "" { target *-*-* } .-1 }
  virtual void m ();
};

struct S3_2 // { dg-warning "'trivial_abi' cannot be applied to 'S3_2'" }
{	    // { dg-message "is polymorphic" "" { target *-*-* } .-1 }
  virtual void m ();
} __attribute__ ((trivial_abi));

// clang-format off
struct __attribute__ ((trivial_abi))
S3_3 // { dg-warning "'trivial_abi' cannot be applied to 'S3_3'" }
{    // { dg-message "has a non-static data member of non-trivial type 'S3_2'" "" { target *-*-* } .-1 }
  S3_3 (S3_3 &&);
  S3_2 s32;
};
// clang-format on

// Diagnose invalid trivial_abi even when the type is templated because it has a
// non-trivial field. Note: GCC only diagnoses this at template instantiation,
// unlike Clang which diagnoses it earlier.
template <class T> struct __attribute__ ((trivial_abi)) S3_4
{
  S3_4 (S3_4 &&);
  S3_2 s32;
};

struct S4
{
  int a;
};
static_assert (__is_trivially_copyable (S4), "");

struct __attribute__ ((
  trivial_abi)) S5 // { dg-warning "'trivial_abi' cannot be applied to 'S5'" }
  : public virtual S4
// { dg-message "has a virtual base" "" { target *-*-* } .-2 }
{
};

struct __attribute__ ((trivial_abi)) S9 : public S4
{
};

struct __attribute__ ((trivial_abi (1)))
S8 // { dg-error "wrong number of arguments" }
{  // { dg-message "expected 0, found 1" "" { target *-*-* } .-1 }
  int a;
};

// Do not warn about deleted ctors when 'trivial_abi' is used to annotate a
// template class.
template <class T> struct __attribute__ ((trivial_abi)) S10
{
  T p;
};

S10<int *> p1;

template <class T> struct S14
{
  T a;
};

template <class T> struct __attribute__ ((trivial_abi)) S15 : S14<T>
{
};

S15<int> s15;

template <class T> struct __attribute__ ((trivial_abi)) S16
{
  S14<T> a;
};

S16<int> s16;

template <class T> struct __attribute__ ((trivial_abi)) S17
{
};

S17<int> s17;

// clang-format off
namespace deletedCopyMoveConstructor {
struct __attribute__ ((trivial_abi))
CopyMoveDeleted                    // { dg-warning "cannot be applied" }
{                                  // { dg-message "copy constructors and move constructors are all deleted" "" { target *-*-* } .-1 }
  CopyMoveDeleted (const CopyMoveDeleted &) = delete;
  CopyMoveDeleted (CopyMoveDeleted &&) = delete;
};

struct __attribute__ ((trivial_abi)) S18 // { dg-warning "cannot be applied" }
{                                         // { dg-message "has a non-static data member of non-trivial type 'deletedCopyMoveConstructor::CopyMoveDeleted'" "" { target *-*-* } .-1 }
  CopyMoveDeleted a;
};
// clang-format on

struct __attribute__ ((trivial_abi)) CopyDeleted
{
  CopyDeleted (const CopyDeleted &) = delete;
  CopyDeleted (CopyDeleted &&) = default;
};

struct __attribute__ ((trivial_abi)) MoveDeleted
{
  MoveDeleted (const MoveDeleted &) = default;
  MoveDeleted (MoveDeleted &&) = delete;
};

// clang-format off
struct __attribute__ ((trivial_abi)) S19 // { dg-warning "cannot be applied" }
{                                         // { dg-message "copy constructors and move constructors are all deleted" "" { target *-*-* } .-1 }
  CopyDeleted a;
  MoveDeleted b;
};
// clang-format on

// This is fine since the move constructor isn't deleted.
struct __attribute__ ((trivial_abi)) S20
{
  int &&a; // a member of rvalue reference type deletes the copy constructor.
};

} // namespace deletedCopyMoveConstructor
