enum Foo {
    X, Y(usize, usize)
}

fn distinct_variant() {
    let mut y = Foo::Y(1, 2);

    let a = match y {
      Foo::Y(ref mut a, _) => a,
      Foo::X => panic!()
    };

    // While `a` and `b` are disjoint, borrowck doesn't know that `a` is not
    // also used for the discriminant of `Foo`, which it would be if `a` was a
    // reference.
    let b = match y {
      Foo::Y(_, ref mut b) => b,
// { dg-error ".E0503." "" { target *-*-* } .-1 }
      Foo::X => panic!()
    };

    *a += 1;
    *b += 1;
}

fn same_variant() {
    let mut y = Foo::Y(1, 2);

    let a = match y {
      Foo::Y(ref mut a, _) => a,
      Foo::X => panic!()
    };

    let b = match y {
      Foo::Y(ref mut b, _) => b, // { dg-error ".E0499." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
      Foo::X => panic!()
    };

    *a += 1;
    *b += 1;
}

fn main() {
}

