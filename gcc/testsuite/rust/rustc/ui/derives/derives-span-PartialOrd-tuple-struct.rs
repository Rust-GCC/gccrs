// This file was auto-generated using 'src/etc/generate-deriving-span-tests.py'

#[derive(PartialEq)]
struct Error;

#[derive(PartialOrd,PartialEq)]
struct Struct(
    Error // { dg-error ".E0277." "" { target *-*-* } }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-5 }
);

fn main() {}

