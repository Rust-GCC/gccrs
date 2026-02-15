// run-rustfix

// Regression test for changes introduced while fixing #54505

// This test uses non-literals for Ranges
// (expecting no parens with borrow suggestion)

use std::ops::RangeBounds;


// take a reference to any built-in range
fn take_range(_r: &impl RangeBounds<i8>) {}


fn main() {
    take_range(std::ops::Range { start: 0, end: 1 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(::std::ops::Range { start: 0, end: 1 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(std::ops::RangeFrom { start: 1 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(::std::ops::RangeFrom { start: 1 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(std::ops::RangeFull {});
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(::std::ops::RangeFull {});
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(std::ops::RangeInclusive::new(0, 1));
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(::std::ops::RangeInclusive::new(0, 1));
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(std::ops::RangeTo { end: 5 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(::std::ops::RangeTo { end: 5 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(std::ops::RangeToInclusive { end: 5 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }

    take_range(::std::ops::RangeToInclusive { end: 5 });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }
}

