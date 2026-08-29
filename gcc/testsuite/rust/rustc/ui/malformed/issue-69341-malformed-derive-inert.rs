fn main() {}

struct CLI {
    #[derive(parse())]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    path: (),
// { dg-error ".E0774." "" { target *-*-* } .-1 }
}

