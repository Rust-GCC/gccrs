fn b(foo: u32, &mut self) { }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

fn main() { }

