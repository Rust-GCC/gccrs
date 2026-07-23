fn main() {
    let s = extern::foo::Bar; // { dg-error "" "" { target *-*-* } }
}

