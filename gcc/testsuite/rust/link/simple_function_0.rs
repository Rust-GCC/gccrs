// TODO: supply simple_function_1
// { dg-excess-errors "" { xfail *-*-* } }
extern crate simple_function_1;
use simple_function_1::test_func;

fn main() -> i32 {
    let a = test_func(123);
    // \{ d//g-bogus "call to extern function" "" { xfail *-*-* } .-1 }
    a - 124
}
