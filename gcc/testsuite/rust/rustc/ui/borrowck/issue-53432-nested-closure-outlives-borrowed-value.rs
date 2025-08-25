fn main() {
    let f = move || {};
    let _action = move || {
        || f() // The `nested` closure
// { dg-error "" "" { target *-*-* } .-1 }
    };
}

