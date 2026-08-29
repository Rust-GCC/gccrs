fn main() {}

trait T {
    fn qux() -> Option<usize> {
        let _ = if true {
        });
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
        Some(4)
    }

