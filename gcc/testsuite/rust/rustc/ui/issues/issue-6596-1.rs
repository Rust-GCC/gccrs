macro_rules! e {
    ($inp:ident) => (
        $nonexistent
// { dg-error "" "" { target *-*-* } .-1 }
    );
}

fn main() {
    e!(foo);
}

