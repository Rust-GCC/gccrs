#[repr(C)] // { dg-error ".E0517." "" { target *-*-* } }
type Foo = u8;

#[repr(packed)] // { dg-error ".E0517." "" { target *-*-* } }
enum Foo2 {Bar, Baz}

#[repr(u8)] // { dg-error ".E0517." "" { target *-*-* } }
struct Foo3 {bar: bool, baz: bool}

#[repr(C)] // { dg-error ".E0517." "" { target *-*-* } }
impl Foo3 {
}

fn main() {
}

