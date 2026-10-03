struct S<'a, T:'a> {
    o: &'a Option<T>
}

fn main() {
    S { o: &None }; // { dg-error ".E0282." "" { target *-*-* } }
}

