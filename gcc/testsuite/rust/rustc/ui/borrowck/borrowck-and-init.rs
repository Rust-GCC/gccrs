fn main() {
    let i: isize;

    println!("{}", false && { i = 5; true });
    println!("{}", i); // { dg-error ".E0381." "" { target *-*-* } }
}

