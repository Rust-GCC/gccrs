struct BuildData {
    foo: isize,
    foo: isize,
// { dg-error ".E0124." "" { target *-*-* } .-1 }
}

fn main() {
}

