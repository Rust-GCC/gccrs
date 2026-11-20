async fn foo() {
// { dg-error ".E0670." "" { target *-*-* } .-1 }
// { dg-note ".E0670." "" { target *-*-* } .-2 }
// { help ".E0670." "" { target *-*-* } .-3 }
// { dg-note ".E0670." "" { target *-*-* } .-4 }

    let x = async {};
// { dg-error ".E0422." "" { target *-*-* } .-1 }
// { dg-note ".E0422." "" { target *-*-* } .-2 }
    let y = async { // { dg-note "" "" { target *-*-* } }
        let x = 42;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { dg-note "" "" { target *-*-* } .-4 }
        42
    };
    let z = async { // { dg-note "" "" { target *-*-* } }
        42
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { dg-note "" "" { target *-*-* } .-4 }
    };
    y.await;
    z.await;
    x
}

fn main() {}

