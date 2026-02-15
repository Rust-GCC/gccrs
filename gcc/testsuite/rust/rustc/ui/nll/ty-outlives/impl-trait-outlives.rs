// compile-flags:-Zborrowck=mir -Zverbose

#![allow(warnings)]

use std::fmt::Debug;

fn no_region<'a, T>(x: Box<T>) -> impl Debug + 'a
// { dg-error ".E0309." "" { target *-*-* } .-1 }
where
    T: Debug,
{
    x
}

fn correct_region<'a, T>(x: Box<T>) -> impl Debug + 'a
where
    T: 'a + Debug,
{
    x
}

fn wrong_region<'a, 'b, T>(x: Box<T>) -> impl Debug + 'a
// { dg-error ".E0309." "" { target *-*-* } .-1 }
where
    T: 'b + Debug,
{
    x
}

fn outlives_region<'a, 'b, T>(x: Box<T>) -> impl Debug + 'a
where
    T: 'b + Debug,
    'b: 'a,
{
    x
}

fn main() {}

