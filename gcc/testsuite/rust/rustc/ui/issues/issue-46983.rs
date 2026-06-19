fn foo(x: &u32) -> &'static u32 {
    &*x
// { dg-error ".E0621." "" { target *-*-* } .-1 }
}

fn main() {}

