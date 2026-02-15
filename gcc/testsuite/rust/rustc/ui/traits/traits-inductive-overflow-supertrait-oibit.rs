// OIBIT-based version of #29859, supertrait version. Test that using
// a simple OIBIT `..` impl alone still doesn't allow arbitrary bounds
// to be synthesized.

#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

auto trait Magic: Copy {} // { dg-error ".E0568." "" { target *-*-* } }

fn copy<T: Magic>(x: T) -> (T, T) { (x, x) }

#[derive(Debug)]
struct NoClone;

fn main() {
    let (a, b) = copy(NoClone); // { dg-error ".E0277." "" { target *-*-* } }
    println!("{:?} {:?}", a, b);
}

