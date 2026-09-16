// run-pass
// run-rustfix

fn main() {
    let small = [1, 2];
    let big = [0u8; 33];

    // Expressions that should trigger the lint
    small.into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    [1, 2].into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    big.into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    [0u8; 33].into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    Box::new(small).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    Box::new([1, 2]).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    Box::new(big).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    Box::new([0u8; 33]).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    Box::new(Box::new(small)).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    Box::new(Box::new([1, 2])).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    Box::new(Box::new(big)).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    Box::new(Box::new([0u8; 33])).into_iter();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    // Expressions that should not
    (&[1, 2]).into_iter();
    (&small).into_iter();
    (&[0u8; 33]).into_iter();
    (&big).into_iter();

    for _ in &[1, 2] {}
    (&small as &[_]).into_iter();
    small[..].into_iter();
    std::iter::IntoIterator::into_iter(&[1, 2]);

    #[allow(array_into_iter)]
    [0, 1].into_iter();
}

