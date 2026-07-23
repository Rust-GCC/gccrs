macro_rules! m {
    () => {
        let // { dg-error "" "" { target *-*-* } }
    };
}

extern "C" {
    m!();
}

fn main() {}

