#![feature(trait_alias)]

trait Foo: Iterator<Item = i32> {}
trait Bar: Foo<Item = u32> {} // { dg-error ".E0284." "" { target *-*-* } }

trait I32Iterator = Iterator<Item = i32>;
trait U32Iterator = I32Iterator<Item = u32>; // { dg-error ".E0284." "" { target *-*-* } }

fn main() {
    let _: &dyn I32Iterator<Item = u32>;
}

