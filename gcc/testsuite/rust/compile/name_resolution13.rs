<<<<<<< HEAD
// { dg-additional-options "-frust-name-resolution-2.0 -frust-compile-until=lowering" }
=======
// { dg-options "-frust-name-resolution-2.0" }

pub mod foo {
    pub macro bar() {}
}
>>>>>>> 9cd774074db (Change to name resolution 2.0)

fn foo() {
    let b = 10;
    fn bar() {
        let c = b;
        // { dg-error "cannot find value .b. in this scope .E0425." "" { target *-*-* } .-1 }
    }
}
