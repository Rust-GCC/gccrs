fn foo<'a, T>(_t: T) where T: Into<&'a str> {}

fn main() {
    foo(String::new());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

