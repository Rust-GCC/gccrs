// { dg-excess-errors "Noisy error and debug" }
struct Foo<A> {
    a: A,
}

impl Foo<isize> {
    fn test() -> i32 { // {dg-error "possible candidate" } 
        123
    }

    fn bar(self) -> isize {
        self.a
    }
}

impl Foo<f32> {
    fn test() -> i32 { // {dg-error "possible candidate" }
        123
    }

    fn bar(self) -> f32 {
        self.a
    }
}

fn main() {
    let a: i32 = Foo::test(); // { dg-error "multiple applicable items in scope for: test" }
}

