fn main() {
    let i = 5;
    let index = 6;

    match i {
        0..=index => println!("winner"),
// { dg-error ".E0080." "" { target *-*-* } .-1 }
        _ => println!("hello"),
    }
}

