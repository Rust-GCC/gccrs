fn main() {
    let x = vec![1];
    x[0i32]; // { dg-error ".E0277." "" { target *-*-* } }
}

