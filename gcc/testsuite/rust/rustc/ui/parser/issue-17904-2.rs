struct Bar<T> { x: T } where T: Copy // { dg-error "" "" { target *-*-* } }

fn main() {}

