// aux-build:redundant-semi-proc-macro-def.rs

#![deny(redundant_semicolons)]
extern crate redundant_semi_proc_macro;
use redundant_semi_proc_macro::should_preserve_spans;

#[should_preserve_spans]
fn span_preservation()  {
    let tst = 123;; // { dg-error "" "" { target *-*-* } }
    match tst {
        // Redundant semicolons are parsed as empty tuple exprs
        // for the lint, so ensure the lint doesn't affect
        // empty tuple exprs explicitly in source.
        123 => (),
        _ => ()
    };;; // { dg-error "" "" { target *-*-* } }
}

fn main() {}

