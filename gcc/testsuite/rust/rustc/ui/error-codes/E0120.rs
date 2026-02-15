trait MyTrait { fn foo() {} }

impl Drop for dyn MyTrait {
// { dg-error ".E0120." "" { target *-*-* } .-1 }
    fn drop(&mut self) {}
}

fn main() {
}

