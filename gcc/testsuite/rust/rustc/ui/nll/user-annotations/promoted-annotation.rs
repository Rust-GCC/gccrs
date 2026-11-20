// Test that type annotations are checked in promoted constants correctly.

fn foo<'a>() {
    let x = 0;
    let f = &drop::<&'a i32>;
    f(&x);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
}

fn main() {}

