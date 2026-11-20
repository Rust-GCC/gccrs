#![feature(label_break_value)]

// These are forbidden occurrences of label-break-value

fn labeled_unsafe() {
    unsafe 'b: {} // { dg-error "" "" { target *-*-* } }
}

fn labeled_if() {
    if true 'b: {} // { dg-error "" "" { target *-*-* } }
}

fn labeled_else() {
    if true {} else 'b: {} // { dg-error "" "" { target *-*-* } }
}

fn labeled_match() {
    match false 'b: {} // { dg-error "" "" { target *-*-* } }
}

pub fn main() {}

