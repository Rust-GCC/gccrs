struct Bug {
    A: [(); { *"" }.len()],
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { dg-error ".E0507." "" { target *-*-* } .-2 }
}

fn main() {}

