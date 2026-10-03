// check-pass

fn main() {
    let _ = "Foo"_;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
}

