struct X(isize);

enum Enum {
    Variant1,
    Variant2
}

impl Drop for X {
    fn drop(&mut self) {}
}
impl Drop for Enum {
    fn drop(&mut self) {}
}

fn main() {
    let foo = X(1);
    drop(foo);
    match foo {
        X(1) => (), // { dg-error ".E0382." "" { target *-*-* } }
        _ => unreachable!()
    }

    let e = Enum::Variant2;
    drop(e);
    match e { // { dg-error ".E0382." "" { target *-*-* } }
        Enum::Variant1 => unreachable!(),
        Enum::Variant2 => ()
    }
}

