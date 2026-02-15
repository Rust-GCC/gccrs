// Test that we can recover from mismatched braces in the parser.

trait Foo {
    fn bar() {
        let x = foo(); // { dg-error ".E0425." "" { target *-*-* } }
    ) // { dg-error "" "" { target *-*-* } }
}

fn main() {
    let x = y.;  // { dg-error ".E0425." "" { target *-*-* } }
// { dg-error ".E0425." "" { target *-*-* } .-1 }
}

