// Tests that references to move-by-default values trigger moves when
// they occur as part of various kinds of expressions.

struct Foo<A> { f: A }
fn touch<A>(_a: &A) {}

fn f00() {
    let x = "hi".to_string();
// { dg-note "" "" { target *-*-* } .-1 }
    let _y = Foo { f:x };
// { dg-note "" "" { target *-*-* } .-1 }
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
// { dg-note ".E0382." "" { target *-*-* } .-1 }
}

fn f05() {
    let x = "hi".to_string();
// { dg-note "" "" { target *-*-* } .-1 }
    let _y = Foo { f:(((x))) };
// { dg-note "" "" { target *-*-* } .-1 }
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
// { dg-note ".E0382." "" { target *-*-* } .-1 }
}

fn f10() {
    let x = "hi".to_string();
    let _y = Foo { f:x.clone() };
    touch(&x);
}

fn f20() {
    let x = "hi".to_string();
    let _y = Foo { f:(x).clone() };
    touch(&x);
}

fn f30() {
    let x = "hi".to_string();
    let _y = Foo { f:((x)).clone() };
    touch(&x);
}

fn f40() {
    let x = "hi".to_string();
    let _y = Foo { f:(((((((x)).clone()))))) };
    touch(&x);
}

fn main() {}

