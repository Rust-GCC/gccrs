fn foo(_x: u32) {
    _x = 4;
// { dg-error ".E0384." "" { target *-*-* } .-1 }
}

fn main() {}

