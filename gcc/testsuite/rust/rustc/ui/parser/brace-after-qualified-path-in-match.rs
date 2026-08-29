fn main() {
    match 10 {
        <T as Trait>::Type{key: value} => (),
// { dg-error "" "" { target *-*-* } .-1 }
        _ => (),
    }
}

