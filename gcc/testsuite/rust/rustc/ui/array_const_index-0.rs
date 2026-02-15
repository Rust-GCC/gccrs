const A: &'static [i32] = &[];
const B: i32 = (&A)[1];
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
    let _ = B;
}

