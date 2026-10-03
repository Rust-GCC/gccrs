// check-pass
// compile-flags: --test

#[test]
#[should_panic = "foo"]
fn test1() {
    panic!();
}

#[test]
#[should_panic(expected)]
// { dg-warning "" "" { target *-*-* } .-1 }
fn test2() {
    panic!();
}

#[test]
#[should_panic(expect)]
// { dg-warning "" "" { target *-*-* } .-1 }
fn test3() {
    panic!();
}

#[test]
#[should_panic(expected(foo, bar))]
// { dg-warning "" "" { target *-*-* } .-1 }
fn test4() {
    panic!();
}

#[test]
#[should_panic(expected = "foo", bar)]
// { dg-warning "" "" { target *-*-* } .-1 }
fn test5() {
    panic!();
}

