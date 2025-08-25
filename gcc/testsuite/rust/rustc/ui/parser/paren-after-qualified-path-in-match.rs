fn main() {
    match 10 {
        <T as Trait>::Type(2) => (),
// { dg-error "" "" { target *-*-* } .-1 }
        _ => (),
    }
}

