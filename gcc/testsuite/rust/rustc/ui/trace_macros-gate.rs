// Test that the trace_macros feature gate is on.

fn main() {
    trace_macros!(); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    trace_macros!(true); // { dg-error ".E0658." "" { target *-*-* } }
    trace_macros!(false); // { dg-error ".E0658." "" { target *-*-* } }

    macro_rules! expando {
        ($x: ident) => { trace_macros!($x) } // { dg-error ".E0658." "" { target *-*-* } }
    }

    expando!(true);
}

