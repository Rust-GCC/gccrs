use std::path::Path;

fn main() {
    let path = Path::new("/tmp/foo/bar.txt");
    println!("{}", path);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

