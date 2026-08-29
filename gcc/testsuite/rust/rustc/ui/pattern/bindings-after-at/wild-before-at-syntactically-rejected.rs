// Here we check that `_ @ sub` is syntactically invalid
// and comes with a nice actionable suggestion.

fn main() {}

#[cfg(FALSE)]
fn wild_before_at_is_bad_syntax() {
    let _ @ a = 0;
// { dg-error "" "" { target *-*-* } .-1 }
    let _ @ ref a = 0;
// { dg-error "" "" { target *-*-* } .-1 }
    let _ @ ref mut a = 0;
// { dg-error "" "" { target *-*-* } .-1 }
    let _ @ (a, .., b) = (0, 1, 2, 3);
// { dg-error "" "" { target *-*-* } .-1 }
}

