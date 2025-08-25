// Regression test for #38520. Check that moves of `Foo` are not
// permitted as `Foo` is not copy (even in a static/const
// initializer).

struct Foo(usize);

const fn get(x: Foo) -> usize {
    x.0
}

const X: Foo = Foo(22);
static Y: usize = get(*&X); // { dg-error ".E0507." "" { target *-*-* } }
const Z: usize = get(*&X); // { dg-error ".E0507." "" { target *-*-* } }

fn main() {
}

