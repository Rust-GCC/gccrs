fn test(cond: bool) {
    let v;
    while cond {
        v = 3;
        break;
    }
    println!("{}", v); // { dg-error ".E0381." "" { target *-*-* } }
}

fn main() {
    test(true);
}

