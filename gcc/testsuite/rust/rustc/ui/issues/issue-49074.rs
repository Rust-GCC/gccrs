// Check that unknown attribute error is shown even if there are unresolved macros.

#[marco_use] // typo
// { dg-error "" "" { target *-*-* } .-1 }
mod foo {
    macro_rules! bar {
        () => ();
    }
}

fn main() {
   bar!(); // { dg-error "" "" { target *-*-* } }
}

