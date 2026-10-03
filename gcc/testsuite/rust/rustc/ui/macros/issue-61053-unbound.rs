#![deny(meta_variable_misuse)]

macro_rules! foo {
    () => {};
    ($( $i:ident = $($j:ident),+ );*) => { $( $( $i = $k; )+ )* };
// { dg-error "" "" { target *-*-* } .-1 }
}

macro_rules! bar {
    () => {};
    (test) => {
        macro_rules! nested {
            () => {};
            ($( $i:ident = $($j:ident),+ );*) => { $( $( $i = $k; )+ )* };
// { dg-error "" "" { target *-*-* } .-1 }
        }
    };
    ( $( $i:ident = $($j:ident),+ );* ) => {
        $(macro_rules! $i {
            () => { $( $i = $k)+ }; // { dg-error "" "" { target *-*-* } }
        })*
    };
}

fn main() {
    foo!();
    bar!();
}

