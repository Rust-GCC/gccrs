#![feature(half_open_range_patterns)]

fn main() {}

#[cfg(FALSE)]
fn syntax() {
    match &0 {
        &0.. | _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
        &0..= | _ => {}
// { dg-error ".E0586." "" { target *-*-* } .-1 }
// { dg-error ".E0586." "" { target *-*-* } .-2 }
        &0... | _ => {}
// { dg-error ".E0586." "" { target *-*-* } .-1 }
    }

    match &0 {
        &..0 | _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
        &..=0 | _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
        &...0 | _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    }
}

