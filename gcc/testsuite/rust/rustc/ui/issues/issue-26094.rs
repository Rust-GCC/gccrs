macro_rules! some_macro {
    ($other: expr) => ({
        $other(None) // { dg-note "" "" { target *-*-* } }
    })
}

fn some_function() {} // { dg-note "" "" { target *-*-* } }

fn main() {
    some_macro!(some_function);
// { dg-error ".E0061." "" { target *-*-* } .-1 }
// { dg-note ".E0061." "" { target *-*-* } .-2 }
}

