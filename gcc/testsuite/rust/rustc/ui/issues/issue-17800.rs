enum MyOption<T> {
    MySome(T),
    MyNone,
}

fn main() {
    match MyOption::MySome(42) {
        MyOption::MySome { x: 42 } => (),
// { dg-error ".E0769." "" { target *-*-* } .-1 }
        _ => (),
    }
}

