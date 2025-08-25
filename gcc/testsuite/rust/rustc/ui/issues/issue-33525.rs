fn main() {
    a; // { dg-error ".E0425." "" { target *-*-* } }
    "".lorem; // { dg-error ".E0609." "" { target *-*-* } }
    "".ipsum; // { dg-error ".E0609." "" { target *-*-* } }
}

