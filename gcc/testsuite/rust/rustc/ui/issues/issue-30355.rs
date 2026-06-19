pub struct X([u8]);

pub static Y: &'static X = {
    const Y: &'static [u8] = b"";
    &X(*Y)
// { dg-error ".E0277." "" { target *-*-* } .-1 }
};

fn main() {}

