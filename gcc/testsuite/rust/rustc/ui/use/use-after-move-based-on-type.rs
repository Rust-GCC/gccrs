fn main() {
    let x = "Hello!".to_string();
    let _y = x;
    println!("{}", x); // { dg-error ".E0382." "" { target *-*-* } }
}

