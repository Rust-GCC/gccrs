fn foo() -> i32 {
    4
}
fn main() {
    let x: u16 = foo();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let y: i64 = x + x;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let z: i32 = x + x;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

