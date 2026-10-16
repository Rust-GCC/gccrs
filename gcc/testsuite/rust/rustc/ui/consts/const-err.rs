// build-fail
// compile-flags: -Zforce-overflow-checks=on

#![allow(arithmetic_overflow)]
#![warn(const_err)]

fn black_box<T>(_: T) {
    unimplemented!()
}

const FOO: u8 = [5u8][1];
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {
    black_box((FOO, FOO));
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
}

