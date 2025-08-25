trait A {
    fn foo(&mut self) {}
}

trait B : A {
    fn foo(&mut self) {}
}

fn bar<T: B>(a: &T) {
    a.foo() // { dg-error ".E0034." "" { target *-*-* } }
}

trait C {
    fn foo(&self) {}
}

trait D : C {
    fn foo(&self) {}
}

fn quz<T: D>(a: &T) {
    a.foo() // { dg-error ".E0034." "" { target *-*-* } }
}

trait E : Sized {
    fn foo(self) {}
}

trait F : E {
    fn foo(self) {}
}

fn foo<T: F>(a: T) {
    a.foo() // { dg-error ".E0034." "" { target *-*-* } }
}

fn pass<T: C>(a: &T) {
    a.foo()
}

fn main() {}

