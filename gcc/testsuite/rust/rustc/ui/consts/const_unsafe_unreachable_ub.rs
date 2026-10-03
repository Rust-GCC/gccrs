// build-fail

#![feature(const_fn)]
#![feature(const_unreachable_unchecked)]

const unsafe fn foo(x: bool) -> bool {
    match x {
        true => true,
        false => std::hint::unreachable_unchecked(),
    }
}

#[warn(const_err)]
const BAR: bool = unsafe { foo(false) };

fn main() {
  assert_eq!(BAR, true);
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
}

