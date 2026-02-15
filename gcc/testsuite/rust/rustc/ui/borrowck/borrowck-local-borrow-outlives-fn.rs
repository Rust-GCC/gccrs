fn cplusplus_mode(x: isize) -> &'static isize {
    &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn main() {}

