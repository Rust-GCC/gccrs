pub trait Partial: Copy {
}

pub trait Complete: Partial {
}

impl<T> Partial for T where T: Complete {}
impl<T> Complete for T {} // { dg-error ".E0277." "" { target *-*-* } }

fn main() {}

