macro_rules! ignored_item {
    () => {
        fn foo() {}
        fn bar() {}
        , // { dg-error "" "" { target *-*-* } }
    }
}

macro_rules! ignored_expr {
    () => ( 1,  // { dg-error "" "" { target *-*-* } }

            2 )
}

macro_rules! ignored_pat {
    () => ( 1, 2 ) // { dg-error "" "" { target *-*-* } }
}

ignored_item!();

fn main() {
    ignored_expr!();
    match 1 {
        ignored_pat!() => (),
        _ => (),
    }
}

