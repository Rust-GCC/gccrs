fn f<T>() {}
struct X;

fn main() {
    false == false == false;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    false == 0 < 2;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    f<X>();
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    f<Result<Option<X>, Option<Option<X>>>(1, 2);
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    use std::convert::identity;
    let _ = identity<u8>;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
}

