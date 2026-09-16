#[link(kind = "wonderful_unicorn")] extern {} // { dg-error ".E0459." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
}

