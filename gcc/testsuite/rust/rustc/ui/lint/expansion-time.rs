// check-pass

#[warn(meta_variable_misuse)]
macro_rules! foo {
    ( $($i:ident)* ) => { $($i)+ }; // { dg-warning "" "" { target *-*-* } }
}

#[warn(missing_fragment_specifier)]
macro_rules! m { ($i) => {} } // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }

#[warn(soft_unstable)]
mod benches {
    #[bench] // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
    fn foo() {}
}

#[warn(incomplete_include)]
fn main() {
    // WARN see in the stderr file, the warning points to the included file.
    include!("expansion-time-include.rs");
}

