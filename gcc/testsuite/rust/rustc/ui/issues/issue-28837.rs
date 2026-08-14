struct A;

fn main() {
    let a = A;

    a + a; // { dg-error ".E0369." "" { target *-*-* } }

    a - a; // { dg-error ".E0369." "" { target *-*-* } }

    a * a; // { dg-error ".E0369." "" { target *-*-* } }

    a / a; // { dg-error ".E0369." "" { target *-*-* } }

    a % a; // { dg-error ".E0369." "" { target *-*-* } }

    a & a; // { dg-error ".E0369." "" { target *-*-* } }

    a | a; // { dg-error ".E0369." "" { target *-*-* } }

    a << a; // { dg-error ".E0369." "" { target *-*-* } }

    a >> a; // { dg-error ".E0369." "" { target *-*-* } }

    a == a; // { dg-error ".E0369." "" { target *-*-* } }

    a != a; // { dg-error ".E0369." "" { target *-*-* } }

    a < a; // { dg-error ".E0369." "" { target *-*-* } }

    a <= a; // { dg-error ".E0369." "" { target *-*-* } }

    a > a; // { dg-error ".E0369." "" { target *-*-* } }

    a >= a; // { dg-error ".E0369." "" { target *-*-* } }
}

