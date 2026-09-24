pub fn f(
    /// Comment
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
    id: u8,
    /// Other
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
    a: u8,
) {}

fn bar(id: #[allow(dead_code)] i32) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

fn main() {
    // verify that the parser recovered and properly typechecked the args
    f("", "");
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-note ".E0308." "" { target *-*-* } .-4 }
    bar("");
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
}

