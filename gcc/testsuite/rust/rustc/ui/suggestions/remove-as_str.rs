fn foo1(s: &str) {
    s.as_str();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

fn foo2<'a>(s: &'a str) {
    s.as_str();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

fn foo3(s: &mut str) {
    s.as_str();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

fn foo4(s: &&str) {
    s.as_str();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

fn main() {}

