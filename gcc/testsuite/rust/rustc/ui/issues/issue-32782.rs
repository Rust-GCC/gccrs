macro_rules! bar (
    () => ()
);

macro_rules! foo (
    () => (
        #[allow_internal_unstable] // { dg-error ".E0658." "" { target *-*-* } }
        bar!();
    );
);

foo!();
fn main() {}

