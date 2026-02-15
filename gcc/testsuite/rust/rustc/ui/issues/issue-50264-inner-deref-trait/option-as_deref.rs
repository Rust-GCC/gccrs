fn main() {
    let _result = &Some(42).as_deref();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

