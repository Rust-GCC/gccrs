fn foo(_: u16) {}
fn foo1(_: f64) {}
fn foo2(_: i32) {}

fn main() {
    foo(1u8);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    foo1(2f32);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    foo2(3i16);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

