// This file was auto-generated using 'src/etc/generate-deriving-span-tests.py'


struct Error;

#[derive(PartialEq)]
struct Struct(
    Error // { dg-error ".E0369." "" { target *-*-* } }
// { dg-error ".E0369." "" { target *-*-* } .-1 }
);

fn main() {}

