#[inline(always)] // { dg-error ".E0518." "" { target *-*-* } }
struct Foo;

#[inline(never)] // { dg-error ".E0518." "" { target *-*-* } }
impl Foo {
}

fn main() {
}

