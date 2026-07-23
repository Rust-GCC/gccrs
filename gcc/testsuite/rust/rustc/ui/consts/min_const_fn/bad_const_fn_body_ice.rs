const fn foo(a: i32) -> Vec<i32> {
    vec![1, 2, 3]
// { dg-error ".E0015." "" { target *-*-* } .-1 }
// { dg-error ".E0015." "" { target *-*-* } .-2 }
}

fn main() {}

