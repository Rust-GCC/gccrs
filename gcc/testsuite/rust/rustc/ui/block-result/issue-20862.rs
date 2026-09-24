fn foo(x: i32) {
    |y| x + y
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {
    let x = foo(5)(2);
// { dg-error ".E0618." "" { target *-*-* } .-1 }
}

