// ignore-tidy-cr

pub fn main() {
    br"a"; // { dg-error "" "" { target *-*-* } }
    br"é";  // { dg-error "" "" { target *-*-* } }
    br##~"a"~##;  // { dg-error "" "" { target *-*-* } }
}

