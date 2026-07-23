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
    if let Some(mut x) = s {
        x = S;
    }
    foo(s); // { dg-error ".E0382." "" { target *-*-* } }
    let mut e = E::V { s: S };
    let E::V { s: mut x } = e;
    x = S;
    bar(e); // { dg-error ".E0382." "" { target *-*-* } }
}

