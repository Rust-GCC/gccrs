// Check that the future-compat-lint for the reservation conflict is
// handled like any other lint.

// edition:2018

mod future_compat_allow {
    #![allow(mutable_borrow_reservation_conflict)]

    fn reservation_conflict() {
        let mut v = vec![0, 1, 2];
        let shared = &v;

        v.push(shared.len());
    }
}

mod future_compat_warn {
    #![warn(mutable_borrow_reservation_conflict)]

    fn reservation_conflict() {
        let mut v = vec![0, 1, 2];
        let shared = &v;

        v.push(shared.len());
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    }
}

mod future_compat_deny {
    #![deny(mutable_borrow_reservation_conflict)]

    fn reservation_conflict() {
        let mut v = vec![0, 1, 2];
        let shared = &v;

        v.push(shared.len());
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    }
}

fn main() {}

