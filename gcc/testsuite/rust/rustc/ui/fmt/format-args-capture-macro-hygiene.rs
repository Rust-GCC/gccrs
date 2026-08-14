#![feature(format_args_capture)]

fn main() {
    format!(concat!("{foo}"));         // { dg-error "" "" { target *-*-* } }
    format!(concat!("{ba", "r} {}"), 1);     // { dg-error "" "" { target *-*-* } }
}

