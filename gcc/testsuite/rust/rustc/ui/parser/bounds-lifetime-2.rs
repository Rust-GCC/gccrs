type A = for<'a + 'b> fn(); // { dg-error "" "" { target *-*-* } }

fn main() {}

