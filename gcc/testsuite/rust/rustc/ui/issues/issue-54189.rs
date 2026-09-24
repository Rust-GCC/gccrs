fn bug() -> impl for <'r> Fn() -> &'r () { || { &() } }
// { dg-error ".E0582." "" { target *-*-* } .-1 }

fn main() {
    let f = bug();
}

