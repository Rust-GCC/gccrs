trait NoLifetime {
    fn get<'p, T : Test<'p>>(&self) -> T;
// { dg-note "" "" { target *-*-* } .-1 }
}

trait Test<'p> {
    fn new(buf: &'p mut [u8]) -> Self;
}

struct Foo<'a> {
    buf: &'a mut [u8],
}

impl<'a> Test<'a> for Foo<'a> {
    fn new(buf: &'a mut [u8]) -> Foo<'a> {
        Foo { buf: buf }
    }
}

impl<'a> NoLifetime for Foo<'a> {
    fn get<'p, T: Test<'a> + From<Foo<'a>>>(&self) -> T {
// { dg-error ".E0195." "" { target *-*-* } .-1 }
// { dg-note ".E0195." "" { target *-*-* } .-2 }
        return *self as T;
// { dg-error ".E0605." "" { target *-*-* } .-1 }
// { dg-note ".E0605." "" { target *-*-* } .-2 }
    }
}

fn main() {}

