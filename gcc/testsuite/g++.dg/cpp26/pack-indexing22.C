// PR c++/124487
// { dg-do compile { target c++20 } }
// { dg-options "" }

template <int I, typename... T>
struct A {
  using B = T...[I];				// { dg-warning "pack indexing only available with" "" { target c++23_down } }
  A () requires (requires { B {}; }) {}
};

template <int I, auto... J>
struct C {
  using B = decltype (J...[I]);			// { dg-warning "pack indexing only available with" "" { target c++23_down } }
  C () requires (requires { B {}; }) {}
};

template <int I, auto... J>
decltype (J...[I]) foo () { return 0; }		// { dg-warning "pack indexing only available with" "" { target c++23_down } }

template <int I, typename... T>
T...[I] bar () { return 0; }			// { dg-warning "pack indexing only available with" "" { target c++23_down } }

template <int I, typename... T>
using D = T...[I];				// { dg-warning "pack indexing only available with" "" { target c++23_down } }

template <int I, typename... T>
void baz (D <I, int, T...>) {}

template <auto... J>
decltype (J...[1]) corge () { return 0; }	// { dg-warning "pack indexing only available with" "" { target c++23_down } }

void
qux ()
{
  // { dg-final { scan-assembler "_ZN1AILi3EJilsiEEC1EvQrqXtlDyT0_T_EE" } }
  A <3, int, long, short, int> a;
  // { dg-final { scan-assembler "_ZN1CILi2EJLi0ELx0ELj0ELl0EEEC1EvQrqXtlDtsyT0_T_EEE" } }
  C <2, 0, 0LL, 0U, 0L> c;
  // { dg-final { scan-assembler "_Z3fooILi0ETpTnDaJLi1EEEDtsyT0_T_Ev" } }
  foo <0, 1> ();
  // { dg-final { scan-assembler "_Z3fooILi2ETpTnDaJLj1ELl2ELx3ELd4010000000000000EEEDtsyT0_T_Ev" } }
  foo <2, 1U, 2L, 3LL, 4.0> ();
  // { dg-final { scan-assembler "_Z3barILi0EJiEEDyT0_T_v" } }
  bar <0, int> ();
  // { dg-final { scan-assembler "_Z3barILi3EJjlxdEEDyT0_T_v" } }
  bar <3, unsigned, long, long long, double> ();
//  baz <0, int> ();
  // { dg-final { scan-assembler "_Z5corgeITpTnDaJLi1ELx2ELj3ELl4ELi5EEEDtsyT_L\[jmy]1EEv" } }
  corge <1, 2LL, 3U, 4L, 5> ();
}
