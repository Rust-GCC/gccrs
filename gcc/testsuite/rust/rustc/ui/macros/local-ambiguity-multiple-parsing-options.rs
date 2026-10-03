fn main() {}

macro_rules! ambiguity {
    ($($i:ident)* $j:ident) => {};
}

ambiguity!(error); // { dg-error "" "" { target *-*-* } }
ambiguity!(error); // { dg-error "" "" { target *-*-* } }

