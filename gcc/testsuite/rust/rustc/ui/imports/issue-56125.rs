// edition:2018
// compile-flags:--extern issue_56125
// aux-build:issue-56125.rs

mod m1 {
    use issue_56125::last_segment::*;
// { dg-error ".E0659." "" { target *-*-* } .-1 }
}

mod m2 {
    use issue_56125::non_last_segment::non_last_segment::*;
// { dg-error ".E0659." "" { target *-*-* } .-1 }
}

mod m3 {
    mod empty {}
    use empty::issue_56125; // { dg-error ".E0432." "" { target *-*-* } }
    use issue_56125::*; // { dg-error ".E0659." "" { target *-*-* } }
}

fn main() {}

