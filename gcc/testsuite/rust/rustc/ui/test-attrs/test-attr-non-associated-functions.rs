// #[test] attribute is not allowed on associated functions or methods
// reworded error message
// compile-flags:--test

struct A {}

impl A {
    #[test]
    fn new() -> A {
// { dg-error "" "" { target *-*-* } .-1 }
        A {}
    }
    #[test]
    fn recovery_witness() -> A {
// { dg-error "" "" { target *-*-* } .-1 }
        A {}
    }
}

#[test]
fn test() {
    let _ = A::new();
}

fn main() {}

