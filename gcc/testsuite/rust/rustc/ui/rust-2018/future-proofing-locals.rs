// edition:2018

#![allow(non_camel_case_types)]
#![allow(unused_imports)]

mod T {
    pub struct U;
}
mod x {
    pub struct y;
}

fn type_param<T>() {
    use T as _; // { dg-error "" "" { target *-*-* } }
    use T::U; // { dg-error "" "" { target *-*-* } }
    use T::*; // { dg-error "" "" { target *-*-* } }
}

fn self_import<T>() {
    use T; // { dg-error "" "" { target *-*-* } }
}

fn let_binding() {
    let x = 10;

    use x as _; // { dg-error "" "" { target *-*-* } }
    use x::y; // OK
    use x::*; // OK
}

fn param_binding(x: u8) {
    use x; // { dg-error "" "" { target *-*-* } }
}

fn match_binding() {
    match 0 {
        x => {
            use x; // { dg-error "" "" { target *-*-* } }
        }
    }
}

fn nested<T>() {
    let x = 10;

    use {T as _, x}; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

