fn foo() -> bool {
    false
    //!self.allow_ty_infer()
// { dg-error "" "" { target *-*-* } .-1 }
}

fn bar() -> bool {
    false
    /*! bar */ // { dg-error "" "" { target *-*-* } }
}

fn baz() -> i32 {
    1 /** baz */ // { dg-error "" "" { target *-*-* } }
}

fn quux() -> i32 {
    2 /*! quux */ // { dg-error "" "" { target *-*-* } }
}

fn main() {}

