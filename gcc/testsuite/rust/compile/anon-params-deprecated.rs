// { dg-do compile }
// { dg-options "-frust-edition=2015" }
// { dg-additional-options "-Wall" }
// Test for the anonymous_parameters deprecation lint (RFC 1685)

#![warn(anonymous_parameters)]

#[allow(dead_code)]
trait T {
    fn foo(i32); // { dg-error "expecting .:. but .\\). found" ""  { target *-*-* } }
                 // WARNING this is accepted in the latest rustc edition

    fn bar_with_default_impl(String, String) {}
    // { dg-error "expecting .:. but .,. found" ""  { target *-*-* } .-1 }
    // { dg-error "expecting .\\). but .,. found" ""  { target *-*-* } .-2 }
    // { dg-error "function declaration missing closing parentheses after parameter list" ""  { target *-*-* } .-3 }
}
// { dg-error "failed to parse trait item in trait" ""  { target *-*-* } .-1 }
// { dg-error "failed to parse item in crate" ""  { target *-*-* } .-2 }

fn main() {}
