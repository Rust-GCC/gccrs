// edition:2018

struct S;

impl S {
    #[cfg(FALSE)]
    unsafe async fn g() {} // { dg-error "" "" { target *-*-* } }
}

#[cfg(FALSE)]
unsafe async fn f() {} // { dg-error "" "" { target *-*-* } }

