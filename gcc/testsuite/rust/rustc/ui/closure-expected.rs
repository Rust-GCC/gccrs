fn main() {
    let x = Some(1);
    let y = x.or_else(4);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

