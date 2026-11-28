#![deny(dead_code)]

fn unused() { // { dg-error "" "" { target *-*-* } }
    println!("blah");
}

fn unused2(var: i32) { // { dg-error "" "" { target *-*-* } }
    println!("foo {}", var);
}

fn unused3( // { dg-error "" "" { target *-*-* } }
    var: i32,
) {
    println!("bar {}", var);
}

fn main() {
    println!("Hello world!");
}

