struct Direct<'a> {
    f: &'a isize
}

struct Indirect1 {
    // Here the lifetime parameter of direct is bound by the fn()
    g: Box<dyn FnOnce(Direct) + 'static>
}

struct Indirect2<'a> {
    // But here it is set to 'a
    g: Box<dyn FnOnce(Direct<'a>) + 'static>
}

fn take_direct<'a,'b>(p: Direct<'a>) -> Direct<'b> { p } // { dg-error ".E0308." "" { target *-*-* } }

fn take_indirect1(p: Indirect1) -> Indirect1 { p }

fn take_indirect2<'a,'b>(p: Indirect2<'a>) -> Indirect2<'b> { p } // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-error "" "" { target *-*-* } .-6 }

fn main() {}

