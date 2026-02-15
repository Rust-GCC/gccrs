#![allow(non_camel_case_types)]

trait Qux {}
struct A;
struct B;
impl Qux for A {}
impl Qux for B {}

struct Foo<T, U: Qux = A, V: Qux = B>(T, U, V);

struct foo;
struct bar;

fn want<T>(t: T) {}

fn have_usize(f: usize) {
    want::<foo>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<bar>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
}

fn have_foo(f: foo) {
    want::<usize>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<bar>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
}

fn have_foo_foo(f: Foo<foo>) {
    want::<usize>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<foo>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<bar>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<&Foo<foo>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<&Foo<foo, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
}

fn have_foo_foo_b(f: Foo<foo, B>) {
    want::<usize>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<foo>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<bar>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<&Foo<foo>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<&Foo<foo, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
}

fn have_foo_foo_b_a(f: Foo<foo, B, A>) {
    want::<usize>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<foo>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<bar>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<usize, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<foo, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<Foo<bar, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<&Foo<foo>>(f); // { dg-error ".E0308." "" { target *-*-* } }
    want::<&Foo<foo, B>>(f); // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

