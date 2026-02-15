fn foo() -> &'static u32 {
    let x = 0;
    &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn main() { }

