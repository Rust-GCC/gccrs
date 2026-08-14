fn new<'r, T>() -> &'r T {
    panic!()
}

fn main() {
    let &v = new();
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

