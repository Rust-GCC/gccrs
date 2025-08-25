#![allow(warnings)]

// This test verifies that the suggestion to move types before associated type bindings
// is correct.

trait One<T> {
  type A;
}

trait OneWithLifetime<'a, T> {
  type A;
}

trait Three<T, U, V> {
  type A;
  type B;
  type C;
}

trait ThreeWithLifetime<'a, 'b, 'c, T, U, V> {
  type A;
  type B;
  type C;
}

struct A<T, M: One<A=(), T>> {
// { dg-error "" "" { target *-*-* } .-1 }
    m: M,
    t: T,
}


struct Al<'a, T, M: OneWithLifetime<A=(), T, 'a>> {
// { dg-error ".E0747." "" { target *-*-* } .-1 }
// { dg-error ".E0747." "" { target *-*-* } .-2 }
    m: M,
    t: &'a T,
}

struct B<T, U, V, M: Three<A=(), B=(), C=(), T, U, V>> {
// { dg-error "" "" { target *-*-* } .-1 }
    m: M,
    t: T,
    u: U,
    v: V,
}

struct Bl<'a, 'b, 'c, T, U, V, M: ThreeWithLifetime<A=(), B=(), C=(), T, U, V, 'a, 'b, 'c>> {
// { dg-error ".E0747." "" { target *-*-* } .-1 }
// { dg-error ".E0747." "" { target *-*-* } .-2 }
    m: M,
    t: &'a T,
    u: &'b U,
    v: &'c V,
}

struct C<T, U, V, M: Three<T, A=(), B=(), C=(), U, V>> {
// { dg-error "" "" { target *-*-* } .-1 }
    m: M,
    t: T,
    u: U,
    v: V,
}

struct Cl<'a, 'b, 'c, T, U, V, M: ThreeWithLifetime<T, 'a, A=(), B=(), C=(), U, 'b, V, 'c>> {
// { dg-error ".E0747." "" { target *-*-* } .-1 }
// { dg-error ".E0747." "" { target *-*-* } .-2 }
    m: M,
    t: &'a T,
    u: &'b U,
    v: &'c V,
}

struct D<T, U, V, M: Three<T, A=(), B=(), U, C=(), V>> {
// { dg-error "" "" { target *-*-* } .-1 }
    m: M,
    t: T,
    u: U,
    v: V,
}

struct Dl<'a, 'b, 'c, T, U, V, M: ThreeWithLifetime<T, 'a, A=(), B=(), U, 'b, C=(), V, 'c>> {
// { dg-error ".E0747." "" { target *-*-* } .-1 }
// { dg-error ".E0747." "" { target *-*-* } .-2 }
    m: M,
    t: &'a T,
    u: &'b U,
    v: &'c V,
}

fn main() {}

