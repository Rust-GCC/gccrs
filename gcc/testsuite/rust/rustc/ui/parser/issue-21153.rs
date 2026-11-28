trait MyTrait<T>: Iterator {
    Item = T;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

