#![allow(bare_trait_objects)]

trait Trait {}

fn f<'a, T: Trait + ('a)>() {} // { dg-error "" "" { target *-*-* } }

fn check<'a>() {
    let _: Box<Trait + ('a)>; // { dg-error "" "" { target *-*-* } }
    // FIXME: It'd be great if we could add suggestion to the following case.
    let _: Box<('a) + Trait>; // { dg-error "" "" { target *-*-* } }
}

fn main() {}

