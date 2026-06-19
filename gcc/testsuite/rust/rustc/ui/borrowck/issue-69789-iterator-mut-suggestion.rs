// Regression test for #69789: rustc generated an invalid suggestion
// when `&` reference from `&mut` iterator is mutated.

fn main() {
    for item in &mut std::iter::empty::<&'static ()>() {
// { dg-note "" "" { target *-*-* } .-1 }
        *item = ();
// { dg-error ".E0594." "" { target *-*-* } .-1 }
// { dg-note ".E0594." "" { target *-*-* } .-2 }
    }
}

