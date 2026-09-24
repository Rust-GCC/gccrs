// Test for #56254, we previously allowed the last example on the 2018
// edition. Make sure that we now emit a warning in that case and an error for
// everyone else.

//ignore-compare-mode-nll
//ignore-compare-mode-polonius

//revisions: migrate2015 migrate2018 nll2015 nll2018

//[migrate2018] edition:2018
//[nll2018] edition:2018

#![cfg_attr(any(nll2015, nll2018), feature(nll))]

fn double_conflicts() {
    let mut v = vec![0, 1, 2];
    let shared = &v;

    v.extend(shared);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
}

fn activation_conflict() {
    let mut v = vec![0, 1, 2];

    v.extend(&v);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
}

fn reservation_conflict() {
    let mut v = vec![0, 1, 2];
    let shared = &v;

    v.push(shared.len());
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-2 }

// { dg-warning "" "" { target *-*-* } .-6 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn main() {}

