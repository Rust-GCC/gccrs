type Range = std::ops::Range<usize>;

fn demo(r: &Range) {
    println!("{:?}", r);
}

fn tell(x: usize) -> usize {
    x
}

fn main() {
    demo(tell(1)..tell(10));
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    demo(1..10);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

