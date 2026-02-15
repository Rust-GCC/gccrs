// check-pass

#![warn(unused_must_use)]

#[derive(PartialEq, Eq)]
struct MyStruct {
    n: usize,
}

impl MyStruct {
    #[must_use]
    fn need_to_use_this_method_value(&self) -> usize {
        self.n
    }

    #[must_use]
    fn need_to_use_this_associated_function_value() -> isize {
        -1
    }
}

trait EvenNature {
    #[must_use = "no side effects"]
    fn is_even(&self) -> bool;
}

impl EvenNature for MyStruct {
    fn is_even(&self) -> bool {
        self.n % 2 == 0
    }
}

trait Replaceable {
    fn replace(&mut self, substitute: usize) -> usize;
}

impl Replaceable for MyStruct {
    // ↓ N.b.: `#[must_use]` attribute on a particular trait implementation
    // method won't work; the attribute should be on the method signature in
    // the trait's definition.
    #[must_use]
    fn replace(&mut self, substitute: usize) -> usize {
        let previously = self.n;
        self.n = substitute;
        previously
    }
}

#[must_use = "it's important"]
fn need_to_use_this_value() -> bool {
    false
}

fn main() {
    need_to_use_this_value(); // { dg-warning "" "" { target *-*-* } }

    let mut m = MyStruct { n: 2 };
    let n = MyStruct { n: 3 };

    m.need_to_use_this_method_value(); // { dg-warning "" "" { target *-*-* } }
    m.is_even(); // trait method!
// { dg-warning "" "" { target *-*-* } .-1 }

    MyStruct::need_to_use_this_associated_function_value();
// { dg-warning "" "" { target *-*-* } .-1 }

    m.replace(3); // won't warn (annotation needs to be in trait definition)

    // comparison methods are `must_use`
    2.eq(&3); // { dg-warning "" "" { target *-*-* } }
    m.eq(&n); // { dg-warning "" "" { target *-*-* } }

    // lint includes comparison operators
    2 == 3; // { dg-warning "" "" { target *-*-* } }
    m == n; // { dg-warning "" "" { target *-*-* } }
}

