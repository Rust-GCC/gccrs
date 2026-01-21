macro_rules! test {
    ($a, $b) => {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
        ()
    };
}

fn main() {
    test!()
}

