fn main() {
    let f  = || -> isize {
        let i: isize;
        i // { dg-error ".E0381." "" { target *-*-* } }
    };
    println!("{}", f());
}

