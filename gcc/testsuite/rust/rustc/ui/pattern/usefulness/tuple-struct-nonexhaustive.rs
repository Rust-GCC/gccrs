struct Foo(isize, isize);

fn main() {
    let x = Foo(1, 2);
    match x {   // { dg-error ".E0004." "" { target *-*-* } }
        Foo(1, b) => println!("{}", b),
        Foo(2, b) => println!("{}", b)
    }
}

