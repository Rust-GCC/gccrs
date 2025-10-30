// check-pass

fn macros() {
    macro_rules! foo{
        ($p:pat, $e:expr, $b:block) => {{
            if let $p = $e $b
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
        }}
    }
    macro_rules! bar{
        ($p:pat, $e:expr, $b:block) => {{
            foo!($p, $e, $b)
        }}
    }

    foo!(a, 1, {
        println!("irrefutable pattern");
    });
    bar!(a, 1, {
        println!("irrefutable pattern");
    });
}

pub fn main() {
    if let a = 1 { // { dg-warning "" "" { target *-*-* } }
        println!("irrefutable pattern");
    }

    if let a = 1 { // { dg-warning "" "" { target *-*-* } }
        println!("irrefutable pattern");
    } else if true {
        println!("else-if in irrefutable if-let");
    } else {
        println!("else in irrefutable if-let");
    }

    if let 1 = 2 {
        println!("refutable pattern");
    } else if let a = 1 { // { dg-warning "" "" { target *-*-* } }
        println!("irrefutable pattern");
    }

    if true {
        println!("if");
    } else if let a = 1 { // { dg-warning "" "" { target *-*-* } }
        println!("irrefutable pattern");
    }
}

