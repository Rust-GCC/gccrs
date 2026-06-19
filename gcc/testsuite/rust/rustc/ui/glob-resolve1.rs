// Make sure that globs only bring in public things.

use bar::*;

mod bar {
    use self::fpriv as import;
    fn fpriv() {}
    extern {
        fn epriv();
    }
    enum A { A1 }
    pub enum B { B1 }

    struct C;

    type D = isize;
}

fn foo<T>() {}

fn main() {
    fpriv(); // { dg-error ".E0425." "" { target *-*-* } }
    epriv(); // { dg-error ".E0425." "" { target *-*-* } }
    B; // { dg-error ".E0423." "" { target *-*-* } }
    C; // { dg-error ".E0425." "" { target *-*-* } }
    import(); // { dg-error ".E0425." "" { target *-*-* } }

    foo::<A>(); // { dg-error ".E0412." "" { target *-*-* } }
    foo::<C>(); // { dg-error ".E0412." "" { target *-*-* } }
    foo::<D>(); // { dg-error ".E0412." "" { target *-*-* } }
}

mod other {
    pub fn import() {}
}

