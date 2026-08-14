fn main() {
    let f = |3: isize| println!("hello");
// { dg-error ".E0005." "" { target *-*-* } .-1 }
    f(4);
}

