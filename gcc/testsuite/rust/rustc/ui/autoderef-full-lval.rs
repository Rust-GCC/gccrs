#![feature(box_syntax)]

struct Clam {
    x: Box<isize>,
    y: Box<isize>,
}

struct Fish {
    a: Box<isize>,
}

fn main() {
    let a: Clam = Clam{x: box 1, y: box 2};
    let b: Clam = Clam{x: box 10, y: box 20};
    let z: isize = a.x + b.y;
// { dg-error ".E0369." "" { target *-*-* } .-1 }
    println!("{}", z);
    assert_eq!(z, 21);
    let forty: Fish = Fish{a: box 40};
    let two: Fish = Fish{a: box 2};
    let answer: isize = forty.a + two.a;
// { dg-error ".E0369." "" { target *-*-* } .-1 }
    println!("{}", answer);
    assert_eq!(answer, 42);
}

