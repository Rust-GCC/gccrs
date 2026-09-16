enum X<'a, T, 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
    A(&'a &'b T)
}

fn main() {}

