fn foo() -> isize {
    let x: isize;

    while 1 != 2  {
        break;
        x = 0;
    }

    println!("{}", x); // { dg-error ".E0381." "" { target *-*-* } }

    return 17;
}

fn main() { println!("{}", foo()); }

