// compile-flags: -D path-statements
struct Droppy;

impl Drop for Droppy {
    fn drop(&mut self) {}
}

fn main() {
    let x = 10;
    x; // { dg-error "" "" { target *-*-* } }

    let y = Droppy;
    y; // { dg-error "" "" { target *-*-* } }

    let z = (Droppy,);
    z; // { dg-error "" "" { target *-*-* } }
}

