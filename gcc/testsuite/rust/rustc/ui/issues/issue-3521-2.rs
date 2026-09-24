fn main() {
    let foo = 100;

    static y: isize = foo + 1;
// { dg-error ".E0435." "" { target *-*-* } .-1 }

    println!("{}", y);
}

