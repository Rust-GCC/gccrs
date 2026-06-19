#![feature(trace_macros)]

fn main() {
    trace_macros!(); // { dg-error "" "" { target *-*-* } }
    trace_macros!(1); // { dg-error "" "" { target *-*-* } }
    trace_macros!(ident); // { dg-error "" "" { target *-*-* } }
    trace_macros!(for); // { dg-error "" "" { target *-*-* } }
    trace_macros!(true,); // { dg-error "" "" { target *-*-* } }
    trace_macros!(false 1); // { dg-error "" "" { target *-*-* } }


    // should be fine:
    macro_rules! expando {
        ($x: ident) => { trace_macros!($x) }
    }

    expando!(true);
}

