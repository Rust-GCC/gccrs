fn foo<U>(t: U) {
    let y = t();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
}

struct Bar;

pub fn some_func() {
    let f = Bar();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
}

fn main() {
    foo(|| { 1 });
}

