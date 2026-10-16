#[cfg(FALSE)]
impl S {
    fn f(#[attr]) {} // { dg-error "" "" { target *-*-* } }
}

#[cfg(FALSE)]
impl T for S {
    fn f(#[attr]) {} // { dg-error "" "" { target *-*-* } }
}

#[cfg(FALSE)]
trait T {
    fn f(#[attr]); // { dg-error "" "" { target *-*-* } }
}

fn main() {}

