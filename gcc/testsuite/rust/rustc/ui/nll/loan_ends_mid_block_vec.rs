#![allow(warnings)]
#![feature(rustc_attrs)]

fn main() {
}

fn nll_fail() {
    let mut data = vec!['a', 'b', 'c'];
    let slice = &mut data;
    capitalize(slice);
    data.push('d');
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    data.push('e');
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    data.push('f');
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    capitalize(slice);
}

fn nll_ok() {
    let mut data = vec!['a', 'b', 'c'];
    let slice = &mut data;
    capitalize(slice);
    data.push('d');
    data.push('e');
    data.push('f');
}

fn capitalize(_: &mut [char]) {
}

