// run-rustfix
// Issue #63988
#[derive(Debug)]
struct S;
fn foo(_: Option<S>) {}

enum E {
    V {
        s: S,
    }
}
fn bar(_: E) {}

fn main() {
    let s = Some(S);
    if let Some(x) = s {
        let _ = x;
    }
    foo(s); // { dg-error ".E0382." "" { target *-*-* } }
    let e = E::V { s: S };
    let E::V { s: x } = e;
    let _ = x;
    bar(e); // { dg-error ".E0382." "" { target *-*-* } }
}

