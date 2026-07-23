pub fn foo<T>(s: S<T>, t: S<T>) {
    let _ = s == t; // { dg-error ".E0369." "" { target *-*-* } }
}

struct S<T>(T);

fn main() {}

