fn func(i: i32) {
    i(); // { dg-error ".E0618." "" { target *-*-* } }
}
fn main() {
    let i = 0i32;
    i(); // { dg-error ".E0618." "" { target *-*-* } }
}

