macro_rules! recursive {
    () => (recursive!()) // { dg-error "" "" { target *-*-* } }
}

fn main() {
    recursive!()
}

