enum E {
    V([Box<E>]),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

