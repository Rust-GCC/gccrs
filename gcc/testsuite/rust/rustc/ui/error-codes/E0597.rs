struct Foo<'a> {
    x: Option<&'a u32>,
}

fn main() {
    let mut x = Foo { x: None };
    let y = 0;
    x.x = Some(&y);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
}

impl<'a> Drop for Foo<'a> { fn drop(&mut self) { } }

