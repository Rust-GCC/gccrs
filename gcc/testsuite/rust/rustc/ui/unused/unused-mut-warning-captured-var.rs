// run-rustfix

#![forbid(unused_mut)]

fn main() {
    let mut x = 1;
// { dg-error "" "" { target *-*-* } .-1 }
    (move|| { println!("{}", x); })();
}

