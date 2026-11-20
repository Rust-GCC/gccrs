macro_rules! foo {
    () => {
        #[cfg_attr(all(), unknown)]
// { dg-error "" "" { target *-*-* } .-1 }
        fn foo() {}
    }
}

foo!();

fn main() {}

