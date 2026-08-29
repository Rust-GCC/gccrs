#![feature(trait_alias)]

trait Foo = std::io::Read + std::io::Write;

fn main() {
    let _: Box<dyn std::io::Read + std::io::Write>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
    let _: Box<dyn Foo>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
}

