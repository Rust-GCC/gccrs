fn main() {
    let data: &[u8] = &[0; 10];
    let _: &[i8] = data.into();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

