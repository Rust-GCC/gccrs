#![feature(lint_reasons)]

#![forbid(
    unsafe_code,
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { dg-note "" "" { target *-*-* } .-4 }
// { dg-note "" "" { target *-*-* } .-5 }
// { dg-note "" "" { target *-*-* } .-6 }
    reason = "our errors & omissions insurance policy doesn't cover unsafe Rust"
)]

use std::ptr;

fn main() {
    let a_billion_dollar_mistake = ptr::null();

    #[allow(unsafe_code)]
// { dg-error ".E0453." "" { target *-*-* } .-1 }
// { dg-error ".E0453." "" { target *-*-* } .-2 }
// { dg-error ".E0453." "" { target *-*-* } .-3 }
// { dg-error ".E0453." "" { target *-*-* } .-4 }
// { dg-error ".E0453." "" { target *-*-* } .-5 }
// { dg-error ".E0453." "" { target *-*-* } .-6 }
// { dg-note ".E0453." "" { target *-*-* } .-7 }
// { dg-note ".E0453." "" { target *-*-* } .-8 }
// { dg-note ".E0453." "" { target *-*-* } .-9 }
// { dg-note ".E0453." "" { target *-*-* } .-10 }
// { dg-note ".E0453." "" { target *-*-* } .-11 }
// { dg-note ".E0453." "" { target *-*-* } .-12 }
// { dg-note ".E0453." "" { target *-*-* } .-13 }
// { dg-note ".E0453." "" { target *-*-* } .-14 }
// { dg-note ".E0453." "" { target *-*-* } .-15 }
// { dg-note ".E0453." "" { target *-*-* } .-16 }
// { dg-note ".E0453." "" { target *-*-* } .-17 }
// { dg-note ".E0453." "" { target *-*-* } .-18 }
    unsafe {
        *a_billion_dollar_mistake
    }
}

