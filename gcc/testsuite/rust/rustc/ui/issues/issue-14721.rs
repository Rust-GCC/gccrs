fn main() {
    let foo = "str";
    println!("{}", foo.desc); // { dg-error ".E0609." "" { target *-*-* } }
}

