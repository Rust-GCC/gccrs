fn main() {
    trace_macros!(true); // { dg-error ".E0658." "" { target *-*-* } }
}

