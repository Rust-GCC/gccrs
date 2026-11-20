// compile-flags:-Zborrowck=mir

#![allow(warnings)]

use std::fmt::Debug;

fn no_region<'a, T>(x: Box<T>) -> Box<Debug + 'a>
where
    T: Debug,
{
    x
// { dg-error ".E0309." "" { target *-*-* } .-1 }
}

fn correct_region<'a, T>(x: Box<T>) -> Box<Debug + 'a>
where
    T: 'a + Debug,
{
    x
}

fn wrong_region<'a, 'b, T>(x: Box<T>) -> Box<Debug + 'a>
where
    T: 'b + Debug,
{
    x
// { dg-error ".E0309." "" { target *-*-* } .-1 }
}

fn outlives_region<'a, 'b, T>(x: Box<T>) -> Box<Debug + 'a>
where
    T: 'b + Debug,
    'b: 'a,
{
    x
}

fn main() {}

