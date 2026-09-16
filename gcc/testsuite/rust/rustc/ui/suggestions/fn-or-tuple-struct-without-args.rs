fn foo(a: usize, b: usize) -> usize { a }

fn bar() -> usize { 42 }

struct S(usize, usize);
enum E {
    A(usize),
    B { a: usize },
}
struct V();

trait T {
    fn baz(x: usize, y: usize) -> usize { x }
    fn bat(x: usize) -> usize { 42 }
    fn bax(x: usize) -> usize { 42 }
    fn bach(x: usize) -> usize;
    fn ban(&self) -> usize { 42 }
    fn bal(&self) -> usize;
}

struct X;

impl T for X {
    fn bach(x: usize) -> usize { 42 }
    fn bal(&self) -> usize { 42 }
}

fn main() {
    let _: usize = foo; // { dg-error ".E0308." "" { target *-*-* } }
    let _: S = S; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = bar; // { dg-error ".E0308." "" { target *-*-* } }
    let _: V = V; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = T::baz; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = T::bat; // { dg-error ".E0308." "" { target *-*-* } }
    let _: E = E::A; // { dg-error ".E0308." "" { target *-*-* } }
    let _: E = E::B; // { dg-error ".E0423." "" { target *-*-* } }
    let _: usize = X::baz; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = X::bat; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = X::bax; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = X::bach; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = X::ban; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = X::bal; // { dg-error ".E0308." "" { target *-*-* } }
    let _: usize = X.ban; // { dg-error ".E0615." "" { target *-*-* } }
    let _: usize = X.bal; // { dg-error ".E0615." "" { target *-*-* } }
    let closure = || 42;
    let _: usize = closure; // { dg-error ".E0308." "" { target *-*-* } }
}

