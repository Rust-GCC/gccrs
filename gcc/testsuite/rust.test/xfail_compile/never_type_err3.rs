fn foo() -> i32 {
    let c;
    let d;

    c = if false {
        return 1;
    } else {
        0.0
    };

    d = if true {
        0.0;
    } else {
        return 1;
    };

    0
}

fn bar() -> i32 {
    let a;
    let b;

    // FIXME: Unimplemented features
    a = if true { // { dg-error "expected .T.. got .!." }
    // { dg-error "type resolution failure in AssignmentExpr" "" { target { *-*-* } } .-1 }
        return 0;
    } else {
        return 0;
    };

    // FIXME: Unimplemented features
    b = return 0; // { dg-error "expected .T.. got .!." }
    // { dg-error "type resolution failure in AssignmentExpr" "" { target { *-*-* } } .-1 }
    // { dg-warning "unreachable statement" "" { target { *-*-* } } .-2 }
}

fn baz() -> i32 {
    let mut x = (1.0, return 1, 2.0);

    x.1 = return 2;
    // { dg-warning "unreachable statement" "" { target { *-*-* } } .-1 }

    x.1 = 3; // { dg-error "expected .!. got .<integer>." }
    // { dg-error "type resolution failure in AssignmentExpr" "" { target { *-*-* } } .-1 }
    // { dg-warning "unreachable statement" "" { target { *-*-* } } .-2 }

    x.0 = -1.0;
    // { dg-warning "unreachable statement" "" { target { *-*-* } } .-1 }

    // FIXME: Unimplemented features
    x.0 = return -1; // { dg-error "expected .<float>. got .!." }
    // { dg-error "type resolution failure in AssignmentExpr" "" { target { *-*-* } } .-1 }
    // { dg-warning "unreachable statement" "" { target { *-*-* } } .-2 }
}

fn main() {
    foo();
    bar();
    baz();
}
