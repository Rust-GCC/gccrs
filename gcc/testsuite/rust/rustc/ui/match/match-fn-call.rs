use std::path::Path;

fn main() {
    let path = Path::new("foo");
    match path {
        Path::new("foo") => println!("foo"),
// { dg-error ".E0164." "" { target *-*-* } .-1 }
        Path::new("bar") => println!("bar"),
// { dg-error ".E0164." "" { target *-*-* } .-1 }
        _ => (),
    }
}

