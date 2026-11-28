fn main() {
    let _result = &mut Ok(42).as_deref_mut();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

