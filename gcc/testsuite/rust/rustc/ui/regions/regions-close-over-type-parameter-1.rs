#![feature(box_syntax)]

// Test for what happens when a type parameter `A` is closed over into
// an object. This should yield errors unless `A` (and the object)
// both have suitable bounds.

trait SomeTrait {
    fn get(&self) -> isize;
}

fn make_object1<A: SomeTrait>(v: A) -> Box<dyn SomeTrait + 'static> {
    box v as Box<dyn SomeTrait + 'static>
// { dg-error ".E0309." "" { target *-*-* } .-1 }
}

fn make_object2<'a, A: SomeTrait + 'a>(v: A) -> Box<dyn SomeTrait + 'a> {
    box v as Box<dyn SomeTrait + 'a>
}

fn make_object3<'a, 'b, A: SomeTrait + 'a>(v: A) -> Box<dyn SomeTrait + 'b> {
    box v as Box<dyn SomeTrait + 'b>
// { dg-error ".E0309." "" { target *-*-* } .-1 }
}

fn main() {}

