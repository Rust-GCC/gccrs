#[repr(packed)]
pub struct Bad<T: ?Sized> {
    data: T, // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {}

