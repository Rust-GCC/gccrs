// Issue #14603: Check for references to type parameters from the
// outer scope (in this case, the trait) used on items in an inner
// scope (in this case, the enum).

trait TraitA<A> {
    fn outer(&self) {
        enum Foo<B> {
            Variance(A)
// { dg-error ".E0401." "" { target *-*-* } .-1 }
        }
    }
}

trait TraitB<A> {
    fn outer(&self) {
        struct Foo<B>(A);
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    }
}

trait TraitC<A> {
    fn outer(&self) {
        struct Foo<B> { a: A }
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    }
}

trait TraitD<A> {
    fn outer(&self) {
        fn foo<B>(a: A) { }
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    }
}

fn main() { }

