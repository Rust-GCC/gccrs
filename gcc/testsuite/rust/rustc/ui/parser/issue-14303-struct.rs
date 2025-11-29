struct X<'a, T, 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
    x: &'a &'b T
}

fn main() {}

