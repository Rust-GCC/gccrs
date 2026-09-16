macro_rules! baz(
    baz => () // { dg-error "" "" { target *-*-* } }
);

fn main() {
    baz!(baz);
}

