unsafe extern {
// { dg-error "" "" { target *-*-* } .-1 }
}

unsafe extern "C" {
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

