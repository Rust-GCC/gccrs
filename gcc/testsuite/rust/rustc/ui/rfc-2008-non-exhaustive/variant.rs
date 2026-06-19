// aux-build:variants.rs

extern crate variants;

use variants::NonExhaustiveVariants;

fn main() {
    let variant_struct = NonExhaustiveVariants::Struct { field: 640 };
// { dg-error ".E0639." "" { target *-*-* } .-1 }

    let variant_tuple = NonExhaustiveVariants::Tuple(640);
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    let variant_unit = NonExhaustiveVariants::Unit;
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    match variant_struct {
        NonExhaustiveVariants::Unit => "",
// { dg-error ".E0603." "" { target *-*-* } .-1 }
        NonExhaustiveVariants::Tuple(fe_tpl) => "",
// { dg-error ".E0603." "" { target *-*-* } .-1 }
        NonExhaustiveVariants::Struct { field } => ""
// { dg-error ".E0638." "" { target *-*-* } .-1 }
    };

    if let NonExhaustiveVariants::Tuple(fe_tpl) = variant_struct {
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    }

    if let NonExhaustiveVariants::Struct { field } = variant_struct {
// { dg-error ".E0638." "" { target *-*-* } .-1 }
    }
}

