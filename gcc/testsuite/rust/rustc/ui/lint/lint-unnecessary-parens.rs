// run-rustfix

#![deny(unused_parens)]
#![allow(while_true)] // for rustfix

#[derive(Eq, PartialEq)]
struct X { y: bool }
impl X {
    fn foo(&self, conjunct: bool) -> bool { self.y && conjunct }
}

fn foo() -> isize {
    return (1); // { dg-error "" "" { target *-*-* } }
}
fn bar(y: bool) -> X {
    return (X { y }); // { dg-error "" "" { target *-*-* } }
}

pub fn unused_parens_around_return_type() -> (u32) { // { dg-error "" "" { target *-*-* } }
    panic!()
}

pub fn unused_parens_around_block_return() -> u32 {
    let _foo = {
        (5) // { dg-error "" "" { target *-*-* } }
    };
    (5) // { dg-error "" "" { target *-*-* } }
}

pub trait Trait {
    fn test(&self);
}

pub fn passes_unused_parens_lint() -> &'static (dyn Trait) {
    panic!()
}

macro_rules! baz {
    ($($foo:expr),+) => {
        ($($foo),*)
    }
}

pub const CONST_ITEM: usize = (10); // { dg-error "" "" { target *-*-* } }
pub static STATIC_ITEM: usize = (10); // { dg-error "" "" { target *-*-* } }

fn main() {
    foo();
    bar((true)); // { dg-error "" "" { target *-*-* } }

    if (true) {} // { dg-error "" "" { target *-*-* } }
    while (true) {} // { dg-error "" "" { target *-*-* } }
    match (true) { // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    if let 1 = (1) {} // { dg-error "" "" { target *-*-* } }
    while let 1 = (2) {} // { dg-error "" "" { target *-*-* } }
    let v = X { y: false };
    // struct lits needs parens, so these shouldn't warn.
    if (v == X { y: true }) {}
    if (X { y: true } == v) {}
    if (X { y: false }.y) {}
    // this shouldn't warn, because the parens are necessary to disambiguate let chains
    if let true = (true && false) {}

    while (X { y: false }.foo(true)) {}
    while (true | X { y: false }.y) {}

    match (X { y: false }) {
        _ => {}
    }

    X { y: false }.foo((true)); // { dg-error "" "" { target *-*-* } }

    let mut _a = (0); // { dg-error "" "" { target *-*-* } }
    _a = (0); // { dg-error "" "" { target *-*-* } }
    _a += (1); // { dg-error "" "" { target *-*-* } }

    let _a = baz!(3, 4);
    let _b = baz!(3);
}

