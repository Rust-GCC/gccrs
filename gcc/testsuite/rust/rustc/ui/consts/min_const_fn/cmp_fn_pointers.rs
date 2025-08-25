const fn cmp(x: fn(), y: fn()) -> bool {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
    unsafe { x == y }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

