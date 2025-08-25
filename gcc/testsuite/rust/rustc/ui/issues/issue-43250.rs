fn main() {
    let mut y;
    const C: u32 = 0;
    macro_rules! m {
        ($a:expr) => {
            let $a = 0;
        }
    }
    m!(y);
// { dg-error "" "" { target *-*-* } .-1 }
    m!(C);
// { dg-error "" "" { target *-*-* } .-1 }
}

