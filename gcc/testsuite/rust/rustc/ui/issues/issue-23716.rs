static foo: i32 = 0;

fn bar(foo: i32) {}
// { dg-error ".E0530." "" { target *-*-* } .-1 }
// { dg-error ".E0530." "" { target *-*-* } .-2 }

mod submod {
    pub static answer: i32 = 42;
}

use self::submod::answer;

fn question(answer: i32) {}
// { dg-error ".E0530." "" { target *-*-* } .-1 }
// { dg-error ".E0530." "" { target *-*-* } .-2 }
fn main() {
}

