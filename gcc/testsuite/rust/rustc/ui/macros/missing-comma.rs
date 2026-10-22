macro_rules! foo {
    ($a:ident) => ();
    ($a:ident, $b:ident) => ();
    ($a:ident, $b:ident, $c:ident) => ();
    ($a:ident, $b:ident, $c:ident, $d:ident) => ();
    ($a:ident, $b:ident, $c:ident, $d:ident, $e:ident) => ();
}

macro_rules! bar {
    ($lvl:expr, $($arg:tt)+) => {}
}

macro_rules! check {
    ($ty:ty, $expected:expr) => {};
    ($ty_of:expr, $expected:expr) => {};
}

fn main() {
    println!("{}" a);
// { dg-error "" "" { target *-*-* } .-1 }
    foo!(a b);
// { dg-error "" "" { target *-*-* } .-1 }
    foo!(a, b, c, d e);
// { dg-error "" "" { target *-*-* } .-1 }
    foo!(a, b, c d, e);
// { dg-error "" "" { target *-*-* } .-1 }
    foo!(a, b, c d e);
// { dg-error "" "" { target *-*-* } .-1 }
    bar!(Level::Error, );
// { dg-error "" "" { target *-*-* } .-1 }
    check!(<str as Debug>::fmt, "fmt");
    check!(<str as Debug>::fmt, "fmt",);
// { dg-error "" "" { target *-*-* } .-1 }
}

