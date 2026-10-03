struct A<T, 'a> { // { dg-error "" "" { target *-*-* } }
    t: &'a T,
}

struct B<T, 'a, U> { // { dg-error "" "" { target *-*-* } }
    t: &'a T,
    u: U,
}

struct C<T, U, 'a> { // { dg-error "" "" { target *-*-* } }
    t: &'a T,
    u: U,
}

struct D<T, U, 'a, 'b, V, 'c> { // { dg-error "" "" { target *-*-* } }
    t: &'a T,
    u: &'b U,
    v: &'c V,
}

fn main() {}

