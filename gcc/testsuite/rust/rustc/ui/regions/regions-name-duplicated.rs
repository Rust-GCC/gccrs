struct Foo<'a, 'a> { // { dg-error ".E0263." "" { target *-*-* } }
    x: &'a isize
}

fn main() {}

