macro_rules! identity {
    ($i: ident) => (
        $i
    )
}

fn main() {
    let identity!(_) = 10; // { dg-error "" "" { target *-*-* } }
}

