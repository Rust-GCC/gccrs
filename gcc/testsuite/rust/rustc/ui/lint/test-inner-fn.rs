// compile-flags: --test -D unnameable_test_items

#[test]
fn foo() {
    #[test] // { dg-error "" "" { target *-*-* } }
    fn bar() {}
    bar();
}

mod x {
    #[test]
    fn foo() {
        #[test] // { dg-error "" "" { target *-*-* } }
        fn bar() {}
        bar();
    }
}

fn main() {}

