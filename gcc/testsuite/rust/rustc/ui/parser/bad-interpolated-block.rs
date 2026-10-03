#![feature(label_break_value)]

fn main() {}

macro_rules! m {
    ($b:block) => {
        'lab: $b; // { dg-error "" "" { target *-*-* } }
        unsafe $b; // { dg-error "" "" { target *-*-* } }
        |x: u8| -> () $b; // { dg-error "" "" { target *-*-* } }
    }
}

fn foo() {
    m!({});
}

