fn main() {
    format!("{:X}", "3");
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

