// aux-build:privacy_tuple_struct.rs

extern crate privacy_tuple_struct as other;

mod a {
    pub struct A(());
    pub struct B(isize);
    pub struct C(pub isize, isize);
    pub struct D(pub isize);

    fn test() {
        let a = A(());
        let b = B(2);
        let c = C(2, 3);
        let d = D(4);

        let A(()) = a;
        let A(_) = a;
        match a { A(()) => {} }
        match a { A(_) => {} }

        let B(_) = b;
        let B(_b) = b;
        match b { B(_) => {} }
        match b { B(_b) => {} }
        match b { B(1) => {} B(_) => {} }

        let C(_, _) = c;
        let C(_a, _) = c;
        let C(_, _b) = c;
        let C(_a, _b) = c;
        match c { C(_, _) => {} }
        match c { C(_a, _) => {} }
        match c { C(_, _b) => {} }
        match c { C(_a, _b) => {} }

        let D(_) = d;
        let D(_d) = d;
        match d { D(_) => {} }
        match d { D(_d) => {} }
        match d { D(1) => {} D(_) => {} }

        let a2 = A;
        let b2 = B;
        let c2 = C;
        let d2 = D;
    }
}

fn this_crate() {
    let a = a::A(()); // { dg-error ".E0603." "" { target *-*-* } }
    let b = a::B(2); // { dg-error ".E0603." "" { target *-*-* } }
    let c = a::C(2, 3); // { dg-error ".E0603." "" { target *-*-* } }
    let d = a::D(4);

    let a::A(()) = a; // { dg-error ".E0603." "" { target *-*-* } }
    let a::A(_) = a; // { dg-error ".E0603." "" { target *-*-* } }
    match a { a::A(()) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match a { a::A(_) => {} } // { dg-error ".E0603." "" { target *-*-* } }

    let a::B(_) = b; // { dg-error ".E0603." "" { target *-*-* } }
    let a::B(_b) = b; // { dg-error ".E0603." "" { target *-*-* } }
    match b { a::B(_) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match b { a::B(_b) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match b { a::B(1) => {} a::B(_) => {} } // { dg-error ".E0603." "" { target *-*-* } }
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    let a::C(_, _) = c; // { dg-error ".E0603." "" { target *-*-* } }
    let a::C(_a, _) = c; // { dg-error ".E0603." "" { target *-*-* } }
    let a::C(_, _b) = c; // { dg-error ".E0603." "" { target *-*-* } }
    let a::C(_a, _b) = c; // { dg-error ".E0603." "" { target *-*-* } }
    match c { a::C(_, _) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match c { a::C(_a, _) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match c { a::C(_, _b) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match c { a::C(_a, _b) => {} } // { dg-error ".E0603." "" { target *-*-* } }

    let a::D(_) = d;
    let a::D(_d) = d;
    match d { a::D(_) => {} }
    match d { a::D(_d) => {} }
    match d { a::D(1) => {} a::D(_) => {} }

    let a2 = a::A; // { dg-error ".E0603." "" { target *-*-* } }
    let b2 = a::B; // { dg-error ".E0603." "" { target *-*-* } }
    let c2 = a::C; // { dg-error ".E0603." "" { target *-*-* } }
    let d2 = a::D;
}

fn xcrate() {
    let a = other::A(()); // { dg-error ".E0603." "" { target *-*-* } }
    let b = other::B(2); // { dg-error ".E0603." "" { target *-*-* } }
    let c = other::C(2, 3); // { dg-error ".E0603." "" { target *-*-* } }
    let d = other::D(4);

    let other::A(()) = a; // { dg-error ".E0603." "" { target *-*-* } }
    let other::A(_) = a; // { dg-error ".E0603." "" { target *-*-* } }
    match a { other::A(()) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match a { other::A(_) => {} } // { dg-error ".E0603." "" { target *-*-* } }

    let other::B(_) = b; // { dg-error ".E0603." "" { target *-*-* } }
    let other::B(_b) = b; // { dg-error ".E0603." "" { target *-*-* } }
    match b { other::B(_) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match b { other::B(_b) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match b { other::B(1) => {}// { dg-error ".E0603." "" { target *-*-* } }
        other::B(_) => {} }    // { dg-error ".E0603." "" { target *-*-* } }

    let other::C(_, _) = c; // { dg-error ".E0603." "" { target *-*-* } }
    let other::C(_a, _) = c; // { dg-error ".E0603." "" { target *-*-* } }
    let other::C(_, _b) = c; // { dg-error ".E0603." "" { target *-*-* } }
    let other::C(_a, _b) = c; // { dg-error ".E0603." "" { target *-*-* } }
    match c { other::C(_, _) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match c { other::C(_a, _) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match c { other::C(_, _b) => {} } // { dg-error ".E0603." "" { target *-*-* } }
    match c { other::C(_a, _b) => {} } // { dg-error ".E0603." "" { target *-*-* } }

    let other::D(_) = d;
    let other::D(_d) = d;
    match d { other::D(_) => {} }
    match d { other::D(_d) => {} }
    match d { other::D(1) => {} other::D(_) => {} }

    let a2 = other::A; // { dg-error ".E0603." "" { target *-*-* } }
    let b2 = other::B; // { dg-error ".E0603." "" { target *-*-* } }
    let c2 = other::C; // { dg-error ".E0603." "" { target *-*-* } }
    let d2 = other::D;
}

fn main() {}

