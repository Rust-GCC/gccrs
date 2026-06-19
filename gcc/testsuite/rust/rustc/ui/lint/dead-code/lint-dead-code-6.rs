#![deny(dead_code)]

struct UnusedStruct; // { dg-error "" "" { target *-*-* } }
impl UnusedStruct {
    fn unused_impl_fn_1() { // { dg-error "" "" { target *-*-* } }
        println!("blah");
    }

    fn unused_impl_fn_2(var: i32) { // { dg-error "" "" { target *-*-* } }
        println!("foo {}", var);
    }

    fn unused_impl_fn_3( // { dg-error "" "" { target *-*-* } }
        var: i32,
    ) {
        println!("bar {}", var);
    }
}

fn main() {}

