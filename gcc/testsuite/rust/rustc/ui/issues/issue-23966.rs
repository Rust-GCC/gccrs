fn main() {
    "".chars().fold(|_, _| (), ());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

