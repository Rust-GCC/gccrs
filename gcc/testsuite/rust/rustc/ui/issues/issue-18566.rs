use std::ops::Deref;

struct MyPtr<'a>(&'a mut usize);
impl<'a> Deref for MyPtr<'a> {
    type Target = usize;

    fn deref<'b>(&'b self) -> &'b usize { self.0 }
}

trait Tr {
    fn poke(&self, s: &mut usize);
}

impl Tr for usize {
    fn poke(&self, s: &mut usize)  {
        *s = 2;
    }
}

fn main() {
    let s = &mut 1;

    MyPtr(s).poke(s);
// { dg-error ".E0499." "" { target *-*-* } .-1 }
}

