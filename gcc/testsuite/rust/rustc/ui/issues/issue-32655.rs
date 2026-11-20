macro_rules! foo (
    () => (
        #[derive_Clone] // { dg-error "" "" { target *-*-* } }
        struct T;
    );
);

macro_rules! bar (
    ($e:item) => ($e)
);

foo!();

bar!(
    #[derive_Clone] // { dg-error "" "" { target *-*-* } }
    struct S;
);

fn main() {}

