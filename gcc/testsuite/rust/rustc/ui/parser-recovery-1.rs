// Test that we can recover from missing braces in the parser.

trait Foo {
    fn bar() {
        let x = foo();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
}

fn main() {
    let x = y.;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
} // { dg-error "" "" { target *-*-* } }

