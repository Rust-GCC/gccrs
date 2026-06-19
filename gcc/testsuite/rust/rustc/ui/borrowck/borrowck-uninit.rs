fn foo(x: isize) { println!("{}", x); }

fn main() {
    let x: isize;
    foo(x); // { dg-error ".E0381." "" { target *-*-* } }
}

