#![feature(label_break_value)]
#![allow(unused_labels)]

// Simple continue pointing to an unlabeled break should yield in an error
fn continue_simple() {
    'b: {
        continue; // { dg-error ".E0695." "" { target *-*-* } }
    }
}

// Labeled continue pointing to an unlabeled break should yield in an error
fn continue_labeled() {
    'b: {
        continue 'b; // { dg-error ".E0696." "" { target *-*-* } }
    }
}

// Simple continue that would cross a labeled block should yield in an error
fn continue_crossing() {
    loop {
        'b: {
            continue; // { dg-error ".E0695." "" { target *-*-* } }
        }
    }
}

pub fn main() {}

