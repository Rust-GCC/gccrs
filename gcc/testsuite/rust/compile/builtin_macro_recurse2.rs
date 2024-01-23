// { dg-additional-options "-fdump-tree-gimple" }
#![feature(rustc_attrs)]

#[rustc_builtin_macro]
macro_rules! concat {
    () => {};
}

macro_rules! a {
    () => {
        "hey"
    };
    ($($t:tt)*) => {
        "ho"
    };
}

fn main() {
    // { dg-final { scan-assembler {"abheyho"} } }
    let _ = concat!("a", 'b', a!(), a!(b c d e f a!()), '\0');
}
