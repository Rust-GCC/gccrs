// run-pass

#[allow(dead_code)]
fn macros() {
    macro_rules! foo{
        ($p:pat, $e:expr, $b:block) => {{
            while let $p = $e $b
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
        }}
    }
    macro_rules! bar{
        ($p:pat, $e:expr, $b:block) => {{
            foo!($p, $e, $b)
        }}
    }

    foo!(_a, 1, {
        println!("irrefutable pattern");
    });
    bar!(_a, 1, {
        println!("irrefutable pattern");
    });
}

pub fn main() {
    while let _a = 1 { // { dg-warning "" "" { target *-*-* } }
        println!("irrefutable pattern");
        break;
    }
}

