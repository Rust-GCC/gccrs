#![allow(bare_trait_objects)]

trait Trait<'a> {}

fn main() {
    let _: &for<'a> Trait<'a> + 'static;
// { dg-error ".E0178." "" { target *-*-* } .-1 }
// { help ".E0178." "" { target *-*-* } .-2 }
// { suggestion ".E0178." "" { target *-*-* } .-3 }
}

