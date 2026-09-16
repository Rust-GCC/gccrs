fn main() {
    let box x = Box::new('c'); // { dg-error ".E0658." "" { target *-*-* } }
    println!("x: {}", x);
}

