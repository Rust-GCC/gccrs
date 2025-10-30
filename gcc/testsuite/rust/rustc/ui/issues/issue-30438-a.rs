// Original regression test for Issue #30438.

use std::ops::Index;

struct Test<'a> {
    s: &'a String
}

impl <'a> Index<usize> for Test<'a> {
    type Output = Test<'a>;
    fn index(&self, _: usize) -> &Self::Output {
        return &Test { s: &self.s};
// { dg-error ".E0515." "" { target *-*-* } .-1 }
    }
}

fn main() {
    let s = "Hello World".to_string();
    let test = Test{s: &s};
    let r = &test[0];
    println!("{}", test.s); // OK since test is valid
    println!("{}", r.s); // Segfault since value pointed by r has already been dropped
}

