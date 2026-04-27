fn main() {
    1 + Some(1); // { dg-error ".E0277." "" { target *-*-* } }
    2 as usize - Some(1); // { dg-error ".E0277." "" { target *-*-* } }
    3 * (); // { dg-error ".E0277." "" { target *-*-* } }
    4 / ""; // { dg-error ".E0277." "" { target *-*-* } }
    5 < String::new(); // { dg-error ".E0277." "" { target *-*-* } }
    6 == Ok(1); // { dg-error ".E0277." "" { target *-*-* } }
}

