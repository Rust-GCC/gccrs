use self::Self as Foo; // { dg-error ".E0432." "" { target *-*-* } }

pub fn main() {
    let Self = 5;
// { dg-error ".E0531." "" { target *-*-* } .-1 }

    match 15 {
        Self => (),
// { dg-error ".E0531." "" { target *-*-* } .-1 }
        Foo { x: Self } => (),
// { dg-error ".E0531." "" { target *-*-* } .-1 }
    }
}

