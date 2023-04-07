trait Foo {
    fn f() -> u32;
}

impl Foo for u32 {
    const fn f() -> u32 { 22 } 
        // { dg-error "trait functions cannot be declared const" "" {target *-*-* } .-1 }
        // { dg-error {failed to parse trait impl item in trait impl} "" { target *-*-* } .-2 }
        // { dg-error {failed to parse item in crate} "" { target *-*-* } .-3 }
} 

fn main() { } 
