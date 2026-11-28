trait A {
    fn foo(*mut self); // { dg-error "" "" { target *-*-* } }
    fn baz(*const self); // { dg-error "" "" { target *-*-* } }
    fn bar(*self); // { dg-error "" "" { target *-*-* } }
}

struct X;
impl A for X {
    fn foo(*mut self) { } // { dg-error "" "" { target *-*-* } }
    fn baz(*const self) { } // { dg-error "" "" { target *-*-* } }
    fn bar(*self) { } // { dg-error "" "" { target *-*-* } }
}

fn main() { }

