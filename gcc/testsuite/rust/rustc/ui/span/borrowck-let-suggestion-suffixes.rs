fn id<T>(x: T) -> T { x }

fn f() {
    let old = ['o'];         // statement 0
    let mut v1 = Vec::new(); // statement 1

    let mut v2 = Vec::new(); // statement 2

    {
        let young = ['y'];       // statement 3

        v2.push(&young[0]);      // statement 4
// { dg-error ".E0597." "" { target *-*-* } .-1 }
// { dg-note ".E0597." "" { target *-*-* } .-2 }
    } // { dg-note "" "" { target *-*-* } }

    let mut v3 = Vec::new(); // statement 5

    v3.push(&id('x'));           // statement 6
// { dg-error ".E0716." "" { target *-*-* } .-1 }
// { dg-note ".E0716." "" { target *-*-* } .-2 }
// { dg-note ".E0716." "" { target *-*-* } .-3 }
// { dg-note ".E0716." "" { target *-*-* } .-4 }

    {

        let mut v4 = Vec::new(); // (sub) statement 0

        v4.push(&id('y'));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
// { dg-note ".E0716." "" { target *-*-* } .-2 }
// { dg-note ".E0716." "" { target *-*-* } .-3 }
// { dg-note ".E0716." "" { target *-*-* } .-4 }
        v4.use_ref();
// { dg-note "" "" { target *-*-* } .-1 }
    }                       // (statement 7)

    let mut v5 = Vec::new(); // statement 8

    v5.push(&id('z'));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
// { dg-note ".E0716." "" { target *-*-* } .-2 }
// { dg-note ".E0716." "" { target *-*-* } .-3 }
// { dg-note ".E0716." "" { target *-*-* } .-4 }

    v1.push(&old[0]);

    (v1, v2, v3, /* v4 is above. */ v5).use_ref();
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
}

fn main() {
    f();
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

