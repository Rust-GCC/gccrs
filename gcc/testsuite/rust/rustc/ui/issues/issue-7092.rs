enum Whatever {
}

fn foo(x: Whatever) {
    match x {
        Some(field) =>
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
            field.access(),
    }
}

fn main(){}

