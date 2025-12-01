// This file was auto-generated using 'src/etc/generate-deriving-span-tests.py'


struct Error;

#[derive(Debug)]
enum Enum {
   A {
     x: Error // { dg-error ".E0277." "" { target *-*-* } }
   }
}

fn main() {}

