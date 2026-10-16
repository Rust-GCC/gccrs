struct Foo {
    field1: i32,
    field1: i32,
// { dg-error ".E0124." "" { target *-*-* } .-1 }
}

fn main() {
}

