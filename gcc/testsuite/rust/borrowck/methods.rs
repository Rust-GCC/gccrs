// { dg-additional-options "-frust-compile-until=compilation -frust-borrowcheck" }

#[lang = "sized"]
pub trait Sized {}

struct A {
    x: i32,
}

impl A {
    fn get(&self) -> &i32 {
        &self.x
    }
}

trait B {
    fn get(&self) -> &i32;
    fn set(&mut self, x: i32);
}

impl B for A {
    fn get(&self) -> &i32 {
        &self.x
    }

    fn set(&mut self, x: i32) {
        self.x = x;
    }
}

fn main() {
    let a = A { x: 0 };
    let b = a.get();

    let c: dyn B = A { x: 0 };
    let d = c.get();
    c.set(1);
}