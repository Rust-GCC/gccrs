struct Thing<'a, Q:'a> {
    x: &'a Q
}

fn thing<'a,Q>(x: &Q) -> Thing<'a,Q> {
    Thing { x: x } // { dg-error ".E0621." "" { target *-*-* } }
}

fn main() {
    thing(&());
}

