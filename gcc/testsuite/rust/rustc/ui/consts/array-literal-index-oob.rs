// build-pass
// ignore-pass (test emits codegen-time warnings and verifies that they are not errors)

#![warn(const_err, unconditional_panic)]

fn main() {
    &{ [1, 2, 3][4] };
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
}

