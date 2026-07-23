trait Future {
    type Item;
    type Error;
}

use std::error::Error;

fn foo() -> impl Future<Item=(), Error=Box<dyn Error>> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    Ok(())
}

fn main() {}

