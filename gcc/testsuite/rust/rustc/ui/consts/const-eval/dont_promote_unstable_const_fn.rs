#![unstable(feature = "humans",
            reason = "who ever let humans program computers,
            we're apparently really bad at it",
            issue = "none")]

#![feature(const_fn)]
#![feature(staged_api)]

#[stable(feature = "rust1", since = "1.0.0")]
#[rustc_const_unstable(feature="foo", issue = "none")]
const fn foo() -> u32 { 42 }

fn meh() -> u32 { 42 }

const fn bar() -> u32 { foo() } // { dg-error "" "" { target *-*-* } }

fn a() {
    let _: &'static u32 = &foo(); // { dg-error ".E0716." "" { target *-*-* } }
}

fn main() {
    let _: &'static u32 = &meh(); // { dg-error ".E0716." "" { target *-*-* } }
    let x: &'static _ = &std::time::Duration::from_millis(42).subsec_millis();
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

