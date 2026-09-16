trait A { fn foo(&self); }
trait B { fn foo(&self); }

fn foo<T:A + B>(t: T) {
    t.foo(); // { dg-error ".E0034." "" { target *-*-* } }
}

fn main() {}

