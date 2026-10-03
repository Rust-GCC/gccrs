fn main() {
    assert_eq!(1, 2)
    assert_eq!(3, 4) // { dg-error "" "" { target *-*-* } }
    println!("hello");
}

