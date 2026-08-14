struct A<T>(T);

impl A<&'static u8> {
    fn f() {
        let x = 0;
        Self(&x);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    }
}

fn main() {}

