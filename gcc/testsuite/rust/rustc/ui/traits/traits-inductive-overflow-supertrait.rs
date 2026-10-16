// Regression test for #29859, supertrait version. This example
// allowed arbitrary trait bounds to be synthesized.

trait Magic: Copy {}
impl<T: Magic> Magic for T {}

fn copy<T: Magic>(x: T) -> (T, T) { (x, x) }

#[derive(Debug)]
struct NoClone;

fn main() {
    let (a, b) = copy(NoClone); // { dg-error ".E0275." "" { target *-*-* } }
    println!("{:?} {:?}", a, b);
}

