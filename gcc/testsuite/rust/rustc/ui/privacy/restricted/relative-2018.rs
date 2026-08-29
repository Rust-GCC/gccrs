// edition:2018

mod m {
    pub(in crate) struct S1; // OK
    pub(in super) struct S2; // OK
    pub(in self) struct S3; // OK
    pub(in ::core) struct S4;
// { dg-error ".E0742." "" { target *-*-* } .-1 }
    pub(in a::b) struct S5;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

