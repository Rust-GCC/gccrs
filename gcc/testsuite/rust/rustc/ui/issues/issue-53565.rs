use std::time::{foo, bar, buzz};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
use std::time::{abc, def};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
fn main(){
    println!("Hello World!");
}

