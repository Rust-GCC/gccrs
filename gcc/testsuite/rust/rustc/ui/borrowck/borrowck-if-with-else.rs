fn foo(x: isize) { println!("{}", x); }

fn main() {
    let x: isize;
    if 1 > 2 {
        println!("whoops");
    } else {
        x = 10;
    }
    foo(x); // { dg-error ".E0381." "" { target *-*-* } }
}

