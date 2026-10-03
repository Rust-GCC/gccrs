fn main() {
    let line = String::from("abc");
    let pattern = String::from("bc");
    println!("{:?}", line.find(pattern)); // { dg-error ".E0277." "" { target *-*-* } }
}

