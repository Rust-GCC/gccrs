macro_rules! t {
    () => ( String ; );     // { dg-error "" "" { target *-*-* } }
}

fn main() {
    let i: Vec<t!()>;
}

