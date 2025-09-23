fn foo<'a, 'b, 'a>(x: &'a str, y: &'b str) {
// { dg-error ".E0263." "" { target *-*-* } .-1 }
}

fn main() {}

