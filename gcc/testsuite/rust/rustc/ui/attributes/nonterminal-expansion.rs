// Macros were previously expanded in `Expr` nonterminal tokens, now they are not.

macro_rules! pass_nonterminal {
    ($n:expr) => {
        #[repr(align($n))] // { dg-error ".E0552." "" { target *-*-* } }
// { dg-error ".E0552." "" { target *-*-* } .-2 }
        struct S;
    };
}

macro_rules! n {
    () => { 32 };
}

pass_nonterminal!(n!());

fn main() {}

