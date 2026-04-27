// compile-flags: -F bad-style

fn main() {
    let _InappropriateCamelCasing = true; // { dg-error "" "" { target *-*-* } }
}

