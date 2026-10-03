trait User{
    fn test();
    /// empty doc
// { dg-error ".E0584." "" { target *-*-* } .-1 }
}
fn main() {}

