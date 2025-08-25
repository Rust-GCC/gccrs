// { dg-error "" "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

// build-fail

trait Mirror {
    type It: ?Sized;
}
impl<T: ?Sized> Mirror for T {
    type It = Self;
}
struct S(Option<<S as Mirror>::It>);

fn main() {
// { dg-note "" "" { target *-*-* } .-1 }
    let _s = S(None);
}

