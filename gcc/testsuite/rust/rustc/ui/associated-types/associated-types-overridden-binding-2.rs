#![feature(trait_alias)]

trait I32Iterator = Iterator<Item = i32>;

fn main() {
    let _: &dyn I32Iterator<Item = u32> = &vec![42].into_iter();
// { dg-error ".E0271." "" { target *-*-* } .-1 }
}

