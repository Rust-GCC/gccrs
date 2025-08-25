fn main() {
    [1; <Multiply<Five, Five>>::VAL]; // { dg-error ".E0080." "" { target *-*-* } }
}
trait TypeVal<T> {
    const VAL: T; // { dg-error "" "" { target *-*-* } }
}
struct Five;
struct Multiply<N, M> {
    _n: PhantomData, // { dg-error ".E0412." "" { target *-*-* } }
}
impl<N, M> TypeVal<usize> for Multiply<N, M> where N: TypeVal<VAL> {}
// { dg-error ".E0046." "" { target *-*-* } .-1 }
// { dg-error ".E0046." "" { target *-*-* } .-2 }

