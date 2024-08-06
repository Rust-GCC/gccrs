// { dg-additional-options "-frust-edition=2018" }
trait T {
    fn foo(i32); // { dg-error "expecting .:. but .\\). found" ""  { target *-*-* } }

    fn foo_with_ref(&mut i32); // { dg-error "expecting .:. but .\\). found" ""  { target *-*-* } }

    fn foo_with_qualified_path(<Bar as T>::Baz); // { dg-error "expecting .:. but .\\). found" ""  { target *-*-* } }

    fn foo_with_qualified_path_and_ref(&<Bar as T>::Baz); // { dg-error "expecting .:. but .\\). found" ""  { target *-*-* } }

    fn foo_with_multiple_qualified_paths(<Bar as T>::Baz, <Bar as T>::Baz);
    // { dg-error "expecting .:. but .,. found" ""  { target *-*-* } .-1 }
    // { dg-error "expecting .\\). but .,. found" ""  { target *-*-* } .-2 }
    // { dg-error "function declaration missing closing parentheses after parameter list" ""  { target *-*-* } .-3 }

    fn bar_with_default_impl(String, String) {}
    // { TODO "expecting .:. but .,. found" ""  { target *-*-* } .-1 }
    // { TODO "expecting .\\). but .,. found" ""  { target *-*-* } .-2 }
    //~^ ERROR expected one of `:`
    //~| ERROR expected one of `:`

    // do not complain about missing `b`
    fn baz(a: usize, b, c: usize) -> usize {
        // { dg-error "failed to parse trait item in trait" ""  { target *-*-* } .-1 }
        // { dg-error "failed to parse item in crate" ""  { target *-*-* } .-2 }
        a + b + c
    }
}

fn main() {}
