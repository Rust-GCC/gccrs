trait Foo {
    fn bar(&self);
    fn baz(&self) { }
    fn bah(_: Option<&Self>) { }
}

struct BarTy {
    x : isize,
    y : f64,
}

impl BarTy {
    fn a() {}
    fn b(&self) {}
}

impl Foo for *const BarTy {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        a;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl<'a> Foo for &'a BarTy {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        x;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        y;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        a;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        bah;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        b;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl<'a> Foo for &'a mut BarTy {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        x;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        y;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        a;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        bah;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        b;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl Foo for Box<BarTy> {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        bah;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl Foo for *const isize {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        bah;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl<'a> Foo for &'a isize {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        bah;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl<'a> Foo for &'a mut isize {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        bah;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl Foo for Box<isize> {
    fn bar(&self) {
        baz();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        bah;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

fn main() {}

