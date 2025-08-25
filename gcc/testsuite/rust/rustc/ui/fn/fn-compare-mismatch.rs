fn main() {
    fn f() { }
    fn g() { }
    let x = f == g;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

