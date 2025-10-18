fn main() {}

struct Type;

impl Type {
    undef!();
// { dg-error "" "" { target *-*-* } .-1 }
}

