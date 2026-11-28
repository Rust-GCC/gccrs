#[derive(Clone)]
struct InGroup<F> {
    it: It,
// { dg-error ".E0412." "" { target *-*-* } .-1 }
    f: F,
}
fn dates_in_year() -> impl Clone {
    InGroup { f: |d| d }
// { dg-error ".E0063." "" { target *-*-* } .-1 }
}

fn main() {}

