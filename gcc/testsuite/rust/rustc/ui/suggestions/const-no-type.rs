// In the cases below, the type is missing from the `const` and `static` items.
//
// Here, we test that we:
//
// a) Perform parser recovery.
//
// b) Emit a diagnostic with the actual inferred type to RHS of `=` as the suggestion.

fn main() {}

// These will not reach typeck:

#[cfg(FALSE)]
const C2 = 42;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

#[cfg(FALSE)]
static S2 = "abc";
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

#[cfg(FALSE)]
static mut SM2 = "abc";
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

// These will, so the diagnostics should be stolen by typeck:

const C = 42;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

const D = &&42;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

static S = Vec::<String>::new();
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

static mut SM = "abc";
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

