mod a {
    use std::marker::PhantomData;

    enum Bug {
        V = [PhantomData; { [ () ].len() ].len() as isize,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
    }
}

mod b {
    enum Bug {
        V = [Vec::new; { [].len()  ].len() as isize,
// { dg-error ".E0282." "" { target *-*-* } .-1 }
// { dg-error ".E0282." "" { target *-*-* } .-2 }
// { dg-error ".E0282." "" { target *-*-* } .-3 }
// { dg-error ".E0282." "" { target *-*-* } .-4 }
// { dg-error ".E0282." "" { target *-*-* } .-5 }
    }
}

mod c {
    enum Bug {
        V = [Vec::new; { [0].len() ].len() as isize,
// { dg-error ".E0282." "" { target *-*-* } .-1 }
// { dg-error ".E0282." "" { target *-*-* } .-2 }
// { dg-error ".E0282." "" { target *-*-* } .-3 }
// { dg-error ".E0282." "" { target *-*-* } .-4 }
// { dg-error ".E0282." "" { target *-*-* } .-5 }
    }
}

fn main() {}

