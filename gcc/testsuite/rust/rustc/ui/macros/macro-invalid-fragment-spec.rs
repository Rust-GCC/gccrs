macro_rules! foo(
    ($x:foo) => ()
// { dg-error "" "" { target *-*-* } .-1 }
);

fn main() {
    foo!(foo);
}

