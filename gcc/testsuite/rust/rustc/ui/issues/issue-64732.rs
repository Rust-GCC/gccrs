#![allow(unused)]
fn main() {
    let _foo = b'hello\0';
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    let _bar = 'hello';
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
}

