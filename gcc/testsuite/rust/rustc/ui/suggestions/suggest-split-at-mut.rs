fn main() {
    let mut foo = [1, 2, 3, 4];
    let a = &mut foo[2];
    let b = &mut foo[3]; // { dg-error ".E0499." "" { target *-*-* } }
    *a = 5;
    *b = 6;
    println!("{:?} {:?}", a, b);
}

