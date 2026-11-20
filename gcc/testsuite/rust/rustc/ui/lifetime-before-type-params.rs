#![allow(unused)]
fn first<T, 'a, 'b>() {}
// { dg-error "" "" { target *-*-* } .-1 }
fn second<'a, T, 'b>() {}
// { dg-error "" "" { target *-*-* } .-1 }
fn third<T, U, 'a>() {}
// { dg-error "" "" { target *-*-* } .-1 }
fn fourth<'a, T, 'b, U, 'c, V>() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

