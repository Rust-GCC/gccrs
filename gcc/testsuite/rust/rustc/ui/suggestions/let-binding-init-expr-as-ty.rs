pub fn foo(num: i32) -> i32 {
    let foo: i32::from_be(num);
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }
// { dg-error ".E0223." "" { target *-*-* } .-3 }
    foo
}

fn main() {
    let _ = foo(42);
}

