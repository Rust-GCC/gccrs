// run-rustfix

macro_rules! foo {
    () => {
        assert_eq!("A", "A");
        assert_eq!("B", "B");
    }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-2 }
}

fn main() {
    foo!()
// { dg-note "" "" { target *-*-* } .-1 }
}

