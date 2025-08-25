#![feature(generators, generator_trait)]

use std::ops::Generator;
use std::pin::Pin;

fn main() {
   let s = String::from("foo");
   let mut gen = move || {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
       yield s[..];
   };
   Pin::new(&mut gen).resume(());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

