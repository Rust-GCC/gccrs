fn main() {
    let _ : &(dyn Send,) = &((),);
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

