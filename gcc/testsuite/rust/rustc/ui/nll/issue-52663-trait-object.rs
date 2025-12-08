#![feature(box_syntax)]

trait Foo { fn get(&self); }

impl<A> Foo for A {
    fn get(&self) { }
}

fn main() {
    let _ = {
        let tmp0 = 3;
        let tmp1 = &tmp0;
        box tmp1 as Box<dyn Foo + '_>
    };
// { dg-error ".E0597." "" { target *-*-* } .-3 }
}

