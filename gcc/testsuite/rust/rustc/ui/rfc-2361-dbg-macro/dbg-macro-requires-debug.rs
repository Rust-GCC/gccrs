// Test ensuring that `dbg!(expr)` requires the passed type to implement `Debug`.

struct NotDebug;

fn main() {
    let _: NotDebug = dbg!(NotDebug); // { dg-error ".E0277." "" { target *-*-* } }
}

