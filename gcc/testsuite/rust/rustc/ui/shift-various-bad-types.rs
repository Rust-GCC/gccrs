// Test that we can do shifts by any integral type.

struct Panolpy {
    char: char,
    str: &'static str,
}

fn foo(p: &Panolpy) {
    22 >> p.char;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    22 >> p.str;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    22 >> p;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    let x;
    22 >> x; // ambiguity error winds up being suppressed

    22 >> 1;
    // Integer literal types are OK

    // Type of the result follows the LHS, not the RHS:
    let _: i32 = 22_i64 >> 1_i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() {
}

