fn main() {
    let foo = 100;

    #[derive(Debug)]
    enum Stuff {
        Bar = foo
// { dg-error ".E0435." "" { target *-*-* } .-1 }
    }

    println!("{:?}", Stuff::Bar);
}

