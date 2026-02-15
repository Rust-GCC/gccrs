struct Foo {
    x: &bool,
// { dg-error ".E0106." "" { target *-*-* } .-1 }
}
enum Bar {
    A(u8),
    B(&bool),
// { dg-error ".E0106." "" { target *-*-* } .-1 }
}
type MyStr = &str;
// { dg-error ".E0106." "" { target *-*-* } .-1 }

struct Baz<'a>(&'a str);
struct Buzz<'a, 'b>(&'a str, &'b str);

struct Quux {
    baz: Baz,
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
    buzz: Buzz,
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
}

fn main() {
}

