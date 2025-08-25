trait T { m!(); } // { dg-error "" "" { target *-*-* } }

struct S;
impl S { m!(); } // { dg-error "" "" { target *-*-* } }

fn main() {}

