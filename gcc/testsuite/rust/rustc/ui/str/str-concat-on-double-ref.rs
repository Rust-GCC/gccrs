fn main() {
    let a: &String = &"1".to_owned();
    let b: &str = &"2";
    let c = a + b;
// { dg-error ".E0369." "" { target *-*-* } .-1 }
    println!("{:?}", c);
}

