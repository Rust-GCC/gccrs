// revisions: noopt opt opt_with_overflow_checks
//[noopt]compile-flags: -C opt-level=0
//[opt]compile-flags: -O
//[opt_with_overflow_checks]compile-flags: -C overflow-checks=on -O

// build-pass
// ignore-pass (test emits codegen-time warnings and verifies that they are not errors)

#![warn(const_err, arithmetic_overflow, unconditional_panic)]

fn main() {
    println!("{}", 0u32 - 1);
// { dg-warning "" "" { target *-*-* } .-1 }
    let _x = 0u32 - 1;
// { dg-warning "" "" { target *-*-* } .-1 }
    println!("{}", 1 / (1 - 1));
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
    let _x = 1 / (1 - 1);
// { dg-warning "" "" { target *-*-* } .-1 }
    println!("{}", 1 / (false as u32));
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
    let _x = 1 / (false as u32);
// { dg-warning "" "" { target *-*-* } .-1 }
}

