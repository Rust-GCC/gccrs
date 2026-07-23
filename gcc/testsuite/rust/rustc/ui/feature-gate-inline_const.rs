fn main() {
    let _ = const {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
        true
    };
}

