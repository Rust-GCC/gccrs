// compile-flags: -D bad-style

fn main() {
    let _InappropriateCamelCasing = true; // { dg-error "" "" { target *-*-* } }
}

