#![feature(format_args_capture)]

fn main() {
    format!("{} {foo} {} {bar} {}", 1, 2, 3);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }

    format!("{foo}"); // { dg-error ".E0425." "" { target *-*-* } }

    format!("{valuea} {valueb}", valuea=5, valuec=7);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }

    format!(r##"

        {foo}

    "##);
// { dg-error ".E0425." "" { target *-*-* } .-3 }

    panic!("{foo} {bar}", bar=1); // { dg-error ".E0425." "" { target *-*-* } }
}

