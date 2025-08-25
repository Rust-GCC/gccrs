// normalize-stderr-test: "existed:.*\(" -> "existed: $$FILE_NOT_FOUND_MSG ("

// test that errors in a (selection) of macros don't kill compilation
// immediately, so that we get more errors listed at a time.

#![feature(asm, llvm_asm)]
#![feature(trace_macros, concat_idents)]

#[derive(Default)] // { dg-error ".E0665." "" { target *-*-* } }
enum OrDeriveThis {}

fn main() {
    asm!(invalid); // { dg-error "" "" { target *-*-* } }
    llvm_asm!(invalid); // { dg-error "" "" { target *-*-* } }

    concat_idents!("not", "idents"); // { dg-error "" "" { target *-*-* } }

    option_env!(invalid); // { dg-error "" "" { target *-*-* } }
    env!(invalid); // { dg-error "" "" { target *-*-* } }
    env!(foo, abr, baz); // { dg-error "" "" { target *-*-* } }
    env!("RUST_HOPEFULLY_THIS_DOESNT_EXIST"); // { dg-error "" "" { target *-*-* } }

    format!(invalid); // { dg-error "" "" { target *-*-* } }

    include!(invalid); // { dg-error "" "" { target *-*-* } }

    include_str!(invalid); // { dg-error "" "" { target *-*-* } }
    include_str!("i'd be quite surprised if a file with this name existed"); // { dg-error "" "" { target *-*-* } }
    include_bytes!(invalid); // { dg-error "" "" { target *-*-* } }
    include_bytes!("i'd be quite surprised if a file with this name existed"); // { dg-error "" "" { target *-*-* } }

    trace_macros!(invalid); // { dg-error "" "" { target *-*-* } }
}

