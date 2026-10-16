#![feature(box_syntax)]

fn id<T>(x: T) -> T { x }

trait Foo { }

impl<'a> Foo for &'a isize { }

fn main() {
    let blah;
    {
        let ss: &isize = &id(1);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
        blah = box ss as Box<dyn Foo>;
    }
}

