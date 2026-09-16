struct Foo<'a> {
    s: &'a mut String
}

fn f(x: usize, f: &Foo) {
    f.s.push('x'); // { dg-error ".E0596." "" { target *-*-* } }
}

fn main() {}

