struct S;

impl S {
    pub // { dg-error "" "" { target *-*-* } }
} // { dg-error "" "" { target *-*-* } }

fn main() {}

