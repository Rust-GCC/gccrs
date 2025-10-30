// needed because negating int::MIN will behave differently between
// optimized compilation and unoptimized compilation and thus would
// lead to different lints being emitted

// revisions: noopt opt opt_with_overflow_checks
//[noopt]compile-flags: -C opt-level=0
//[opt]compile-flags: -O
//[opt_with_overflow_checks]compile-flags: -C overflow-checks=on -O

// build-fail

#![feature(rustc_attrs)]

fn black_box<T>(_: T) {
    unimplemented!()
}

fn main() {
    let a = -std::i8::MIN;
// { dg-error "" "" { target *-*-* } .-1 }
    let a_i128 = -std::i128::MIN;
// { dg-error "" "" { target *-*-* } .-1 }
    let b = 200u8 + 200u8 + 200u8;
// { dg-error "" "" { target *-*-* } .-1 }
    let b_i128 = std::i128::MIN - std::i128::MAX;
// { dg-error "" "" { target *-*-* } .-1 }
    let c = 200u8 * 4;
// { dg-error "" "" { target *-*-* } .-1 }
    let d = 42u8 - (42u8 + 1);
// { dg-error "" "" { target *-*-* } .-1 }
    let _e = [5u8][1];
// { dg-error "" "" { target *-*-* } .-1 }
    black_box(a);
    black_box(a_i128);
    black_box(b);
    black_box(b_i128);
    black_box(c);
    black_box(d);
}

