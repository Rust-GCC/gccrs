pub trait Clone2 {
    fn clone(&self) -> Self;
}


trait Getter<T: Clone2> {
    fn get(&self) -> T;
}

impl Getter<isize> for isize { // { dg-error ".E0277." "" { target *-*-* } }
    fn get(&self) -> isize { *self }
}

fn main() { }

