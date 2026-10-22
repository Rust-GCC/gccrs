struct A {
    foo : i32,
    car : i32,
    barr : i32
}

fn main() {
    let a = A {
        foo : 5,
        bar : 42,
// { dg-error ".E0560." "" { target *-*-* } .-1 }
        car : 9,
    };
}

