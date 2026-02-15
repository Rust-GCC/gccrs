#![feature(try_trait)]

fn main() {}

fn foo() -> Result<u32, ()> {
    let x: Option<u32> = None;
    x?; // { dg-error ".E0277." "" { target *-*-* } }
    Ok(22)
}

fn bar() -> u32 {
    let x: Option<u32> = None;
    x?; // { dg-error ".E0277." "" { target *-*-* } }
    22
}

