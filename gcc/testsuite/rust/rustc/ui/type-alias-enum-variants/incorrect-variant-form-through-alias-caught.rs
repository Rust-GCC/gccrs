// ignore-tidy-linelength

// Check that creating/matching on an enum variant through an alias with
// the wrong braced/unit form is caught as an error.

enum Enum { Braced {}, Unit, Tuple() }
type Alias = Enum;

fn main() {
    Alias::Braced;
// { dg-error ".E0533." "" { target *-*-* } .-1 }
    let Alias::Braced = panic!();
// { dg-error ".E0533." "" { target *-*-* } .-1 }
    let Alias::Braced(..) = panic!();
// { dg-error ".E0164." "" { target *-*-* } .-1 }

    Alias::Unit();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
    let Alias::Unit() = panic!();
// { dg-error ".E0164." "" { target *-*-* } .-1 }
}

