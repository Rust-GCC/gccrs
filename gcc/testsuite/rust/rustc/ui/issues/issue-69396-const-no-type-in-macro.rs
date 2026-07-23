macro_rules! suite {
    ( $( $fn:ident; )* ) => {
        $(
            const A = "A".$fn();
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }
// { dg-error ".E0121." "" { target *-*-* } .-3 }
        )*
    }
}

suite! {
    len;
    is_empty;
}

fn main() {}

