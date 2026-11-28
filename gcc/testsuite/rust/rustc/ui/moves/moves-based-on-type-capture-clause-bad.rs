use std::thread;

fn main() {
    let x = "Hello world!".to_string();
    thread::spawn(move|| {
        println!("{}", x);
    });
    println!("{}", x); // { dg-error ".E0382." "" { target *-*-* } }
}

