// N5008 :
// [expr.prim.lambda.closure]/p10
// If all potential references to a local entity implicitly captured by a
// lambda-expression L occur within the function contract assertions of the
// call operator or operator template of L or within assertion-statements
// within the body of L, the program is ill-formed.
// [Note 4: Adding a contract assertion to an existing C++ program cannot
//  cause additional captures. — end note]
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fsyntax-only" }

auto gl0 = [] (int x) 
  pre (x > 10) { return x; }; // OK

static int i = 0;

void
foo ()
{
  auto f1 = [=]
    pre (i > 0) {};  // OK, no local entities are captured.

  int i = 1;

  auto f2 = [=]
    pre (i > 0) {};  // { dg-error {'i' is not implicitly captured by a contract assertion} }

  auto f3 = [i]
    pre (i > 0) {};  // OK, i is captured explicitly.

  auto f4 = [=] {
    contract_assert (i > 0); // { dg-error {'i' is not implicitly captured by a contract assertion} }
  };

  auto f5 = [=] {
    contract_assert (i > 0); // OK, i is referenced elsewhere.
    return i;
  };

  auto f6 = [=] pre (                // #1
    []{
      bool x = true;
      return [=]{ return x; }();    // OK, #1 captures nothing.
    }()) {};

// TODO: lambda captures in function contract specifiers are not yet
// fully functional.
#if 0
  bool y = true;
  auto f7 = [=]
    pre([=]{ return y; }()); // error: outer capture of y is invalid.
#endif
}
