// compile-flags: -W bad-style
// check-pass

fn main() {
    let _InappropriateCamelCasing = true;
// { dg-warning "" "" { target *-*-* } .-1 }
}

