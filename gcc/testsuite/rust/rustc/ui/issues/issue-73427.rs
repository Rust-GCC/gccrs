enum A {
    StructWithFields { x: () },
    TupleWithFields(()),
    Struct {},
    Tuple(),
    Unit,
}

enum B {
    StructWithFields { x: () },
    TupleWithFields(()),
}

enum C {
    StructWithFields { x: () },
    TupleWithFields(()),
    Unit,
}

enum D {
    TupleWithFields(()),
    Unit,
}

fn main() {
    // Only variants without fields are suggested (and others mentioned in a note) where an enum
    // is used rather than a variant.

    A.foo();
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    B.foo();
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    C.foo();
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    D.foo();
// { dg-error ".E0423." "" { target *-*-* } .-1 }

    // Only tuple variants are suggested in calls or tuple struct pattern matching.

    let x = A(3);
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    if let A(3) = x { }
// { dg-error ".E0532." "" { target *-*-* } .-1 }
}

