#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

auto trait Magic : Sized where Option<Self> : Magic {} // { dg-error ".E0568." "" { target *-*-* } }
impl<T:Magic> Magic for T {}

fn copy<T: Magic>(x: T) -> (T, T) { (x, x) }

#[derive(Debug)]
struct NoClone;

fn main() {
    let (a, b) = copy(NoClone);
    println!("{:?} {:?}", a, b);
}

