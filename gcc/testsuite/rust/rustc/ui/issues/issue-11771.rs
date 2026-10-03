fn main() {
    let x = ();
    1 +
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    ;

    let x: () = ();
    1 +
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    ;
}

