// run-rustfix

trait T {
    unsafe fn foo(a: &usize, b: &usize) -> usize;
    fn bar(&self, a: &usize, b: &usize) -> usize;
}

mod foo {
    use super::T;
    impl T for () {} // { dg-error ".E0046." "" { target *-*-* } }

    impl T for usize { // { dg-error ".E0046." "" { target *-*-* } }
    }
}

fn main() {}

