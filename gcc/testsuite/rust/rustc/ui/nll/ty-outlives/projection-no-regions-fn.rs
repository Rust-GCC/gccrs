// compile-flags:-Zborrowck=mir -Zverbose

#![allow(warnings)]

trait Anything { }

impl<T> Anything for T { }

fn no_region<'a, T>(mut x: T) -> Box<dyn Anything + 'a>
where
    T: Iterator,
{
    Box::new(x.next())
// { dg-error ".E0309." "" { target *-*-* } .-1 }
}

fn correct_region<'a, T>(mut x: T) -> Box<dyn Anything + 'a>
where
    T: 'a + Iterator,
{
    Box::new(x.next())
}

fn wrong_region<'a, 'b, T>(mut x: T) -> Box<dyn Anything + 'a>
where
    T: 'b + Iterator,
{
    Box::new(x.next())
// { dg-error ".E0309." "" { target *-*-* } .-1 }
}

fn outlives_region<'a, 'b, T>(mut x: T) -> Box<dyn Anything + 'a>
where
    T: 'b + Iterator,
    'b: 'a,
{
    Box::new(x.next())
}

fn main() {}

