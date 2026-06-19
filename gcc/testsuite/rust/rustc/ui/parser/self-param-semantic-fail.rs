// This test ensures that `self` is semantically rejected
// in contexts with `FnDecl` but outside of associated `fn`s.
// FIXME(Centril): For now closures are an exception.

fn main() {}

fn free() {
    fn f1(self) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn f2(mut self) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn f3(&self) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn f4(&mut self) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn f5<'a>(&'a self) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn f6<'a>(&'a mut self) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn f7(self: u8) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn f8(mut self: u8) {}
// { dg-error "" "" { target *-*-* } .-1 }
}

extern {
    fn f1(self);
// { dg-error "" "" { target *-*-* } .-1 }
    fn f2(mut self);
// { dg-error ".E0130." "" { target *-*-* } .-1 }
// { dg-error ".E0130." "" { target *-*-* } .-2 }
    fn f3(&self);
// { dg-error "" "" { target *-*-* } .-1 }
    fn f4(&mut self);
// { dg-error "" "" { target *-*-* } .-1 }
    fn f5<'a>(&'a self);
// { dg-error "" "" { target *-*-* } .-1 }
    fn f6<'a>(&'a mut self);
// { dg-error "" "" { target *-*-* } .-1 }
    fn f7(self: u8);
// { dg-error "" "" { target *-*-* } .-1 }
    fn f8(mut self: u8);
// { dg-error ".E0130." "" { target *-*-* } .-1 }
// { dg-error ".E0130." "" { target *-*-* } .-2 }
}

type X1 = fn(self);
// { dg-error "" "" { target *-*-* } .-1 }
type X2 = fn(mut self);
// { dg-error ".E0561." "" { target *-*-* } .-1 }
// { dg-error ".E0561." "" { target *-*-* } .-2 }
type X3 = fn(&self);
// { dg-error "" "" { target *-*-* } .-1 }
type X4 = fn(&mut self);
// { dg-error "" "" { target *-*-* } .-1 }
type X5 = for<'a> fn(&'a self);
// { dg-error "" "" { target *-*-* } .-1 }
type X6 = for<'a> fn(&'a mut self);
// { dg-error "" "" { target *-*-* } .-1 }
type X7 = fn(self: u8);
// { dg-error "" "" { target *-*-* } .-1 }
type X8 = fn(mut self: u8);
// { dg-error ".E0561." "" { target *-*-* } .-1 }
// { dg-error ".E0561." "" { target *-*-* } .-2 }

