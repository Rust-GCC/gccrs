fn main() {
    let mut a = "a";
    a += { "b" };
// { dg-error ".E0368." "" { target *-*-* } .-1 }
}

