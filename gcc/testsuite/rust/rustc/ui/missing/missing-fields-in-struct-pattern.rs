struct S(usize, usize, usize, usize);

fn main() {
    if let S { a, b, c, d } = S(1, 2, 3, 4) {
// { dg-error ".E0769." "" { target *-*-* } .-1 }
        println!("hi");
    }
}

