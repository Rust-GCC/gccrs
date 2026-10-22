// run-rustfix

// Regression test for #54505 - range borrowing suggestion had
// incorrect syntax (missing parentheses).

use std::ops::RangeBounds;


// take a reference to any built-in range
fn take_range(_r: &impl RangeBounds<i8>) {}


fn main() {
    take_range(0..1);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(1..);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(..);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(0..=1);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(..5);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(..=42);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }
}

