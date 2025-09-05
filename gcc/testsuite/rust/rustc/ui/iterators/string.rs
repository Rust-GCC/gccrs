fn main() {
    for _ in "".to_owned() {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in "" {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

