struct Empty;

trait Howness {}

impl Howness for () {
    fn how_are_you(&self -> Empty {
// { dg-error ".E0407." "" { target *-*-* } .-1 }
// { dg-error ".E0407." "" { target *-*-* } .-2 }
        Empty
    }
}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

