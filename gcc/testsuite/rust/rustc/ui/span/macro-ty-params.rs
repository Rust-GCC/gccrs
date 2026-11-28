macro_rules! m {
    ($p1: path) => {
        #[derive($p1)] struct U;
    }
}

macro_rules! foo { () => () }

fn main() {
    foo::<T>!(); // { dg-error "" "" { target *-*-* } }
    foo::<>!(); // { dg-error "" "" { target *-*-* } }
    m!(Default<>); // { dg-error "" "" { target *-*-* } }
}

