fn flatten<'a, 'b, T>(x: &'a &'b T) -> &'a T {
    x
}

fn main() {
    let mut x = "original";
    let y = &x;
    let z = &y;
    let w = flatten(z);
    x = "modified";
// { dg-error ".E0506." "" { target *-*-* } .-1 }
    println!("{}", w); // prints "modified"
}

