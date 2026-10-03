impl dyn A {
    Y
} // { dg-error "" "" { target *-*-* } }

struct S;

trait X {
    X() {} // { dg-error "" "" { target *-*-* } }
    fn xxx() { ### }
    L = M;
    Z = { 2 + 3 };
    ::Y ();
}

trait A {
    X() {} // { dg-error "" "" { target *-*-* } }
}
trait B {
    fn xxx() { ### } // { dg-error "" "" { target *-*-* } }
}
trait C {
    L = M; // { dg-error "" "" { target *-*-* } }
}
trait D {
    Z = { 2 + 3 }; // { dg-error "" "" { target *-*-* } }
}
trait E {
    ::Y (); // { dg-error "" "" { target *-*-* } }
}

impl S {
    pub hello_method(&self) { // { dg-error "" "" { target *-*-* } }
        println!("Hello");
    }
}

fn main() {
    S.hello_method(); // { dg-error ".E0599." "" { target *-*-* } }
}

