trait A {}

struct Struct {
    r: dyn A + 'static
}

fn new_struct(r: dyn A + 'static)
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    Struct { r: r }
}

fn main() {}

