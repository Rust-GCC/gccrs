// #12402 Operator overloading only considers the method name, not which trait is implemented

trait MyMul<Rhs, Res> {
    fn mul(&self, rhs: &Rhs) -> Res;
}

fn foo<T: MyMul<f64, f64>>(a: &T, b: f64) -> f64 {
    a * b // { dg-error ".E0369." "" { target *-*-* } }
}

fn main() {}

