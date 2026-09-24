struct GenericAssocMethod<T>(T);

impl<T> GenericAssocMethod<T> {
    fn default_hello() {}
}

fn main() {
    let x = GenericAssocMethod(33i32);
    x.default_hello();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

