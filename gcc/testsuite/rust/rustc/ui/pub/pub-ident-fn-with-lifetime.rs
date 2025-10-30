// run-rustfix

pub   foo<'a>(_s: &'a usize) -> bool { true }
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    foo(&2);
}

