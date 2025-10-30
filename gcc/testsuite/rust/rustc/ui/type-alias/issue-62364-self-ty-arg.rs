struct Struct<P1> {
    field: P1,
}

type Alias<'a> = Struct<&'a Self>;
// { dg-error ".E0411." "" { target *-*-* } .-1 }

fn main() {}

