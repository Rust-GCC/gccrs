struct S<T = u8>(T);
trait Tr<T = u8> {}

impl Self for S {} // { dg-error ".E0411." "" { target *-*-* } }
impl Self::N for S {} // { dg-error ".E0405." "" { target *-*-* } }

fn main() {}

