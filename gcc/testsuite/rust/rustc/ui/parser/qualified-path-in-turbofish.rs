// run-rustfix
trait T {
    type Ty;
}

struct Impl;

impl T for Impl {
    type Ty = u32;
}

fn template<T>() -> i64 {
    3
}

fn main() {
    template::<<Impl as T>:Ty>();
// { dg-error "" "" { target *-*-* } .-1 }
}

