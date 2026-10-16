// run-rustfix

use std::fmt::Debug;

fn foo(d: impl Debug) {
// { help "" "" { target *-*-* } .-1 }
    bar(d);
// { dg-error ".E0310." "" { target *-*-* } .-1 }
// { dg-note ".E0310." "" { target *-*-* } .-2 }
}

fn bar(d: impl Debug + 'static) {
    println!("{:?}", d)
}

fn main() {
  foo("hi");
}

