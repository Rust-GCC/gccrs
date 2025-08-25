#![deny(meta_variable_misuse)]

macro_rules! foo {
    () => {};
    ( $( $i:ident = $($j:ident),+ );* ) => { $( $( $i = $j; )* )* };
// { dg-error "" "" { target *-*-* } .-1 }
    ( $( $($j:ident),+ );* ) => { $( $( $j; )+ )+ }; // { dg-error "" "" { target *-*-* } }
}

macro_rules! bar {
    () => {};
    (test) => {
        macro_rules! nested {
            () => {};
            ( $( $i:ident = $($j:ident),+ );* ) => { $( $( $i = $j; )* )* };
// { dg-error "" "" { target *-*-* } .-1 }
            ( $( $($j:ident),+ );* ) => { $( $( $j; )+ )+ }; // { dg-error "" "" { target *-*-* } }
        }
    };
    ( $( $i:ident = $($j:ident),+ );* ) => {
        $(macro_rules! $i {
            () => { 0 $( + $j )* }; // { dg-error "" "" { target *-*-* } }
        })*
    };
}

fn main() {
    foo!();
    bar!();
}

