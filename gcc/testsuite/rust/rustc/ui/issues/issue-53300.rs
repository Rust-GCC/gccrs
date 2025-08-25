// issue 53300

pub trait A {
    fn add(&self, b: i32) -> i32;
}

fn addition() -> Wrapper<impl A> {}
// { dg-error ".E0412." "" { target *-*-* } .-1 }

fn main() {
    let res = addition();
}

