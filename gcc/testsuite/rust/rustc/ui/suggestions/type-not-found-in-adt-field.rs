struct Struct {
    m: Vec<Someunknownname<String, ()>>, // { dg-error ".E0412." "" { target *-*-* } }
// { dg-note ".E0412." "" { target *-*-* } .-1 }
}
struct OtherStruct { // { help "" "" { target *-*-* } }
    m: K, // { dg-error ".E0412." "" { target *-*-* } }
// { dg-note ".E0412." "" { target *-*-* } .-1 }
}
fn main() {}

