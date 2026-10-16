// This file was auto-generated using 'src/etc/generate-deriving-span-tests.py'


struct Error;

#[derive(Hash)]
struct Struct(
    Error // { dg-error ".E0277." "" { target *-*-* } }
);

fn main() {}

