// ignore-tidy-cr

/// doc comment with bare CR: ''
pub fn foo() {}
// { dg-error "" "" { target *-*-* } .-2 }

/** block doc comment with bare CR: '' */
pub fn bar() {}
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
    //! doc comment with bare CR: ''
// { dg-error "" "" { target *-*-* } .-1 }

    /*! block doc comment with bare CR: '' */
// { dg-error "" "" { target *-*-* } .-1 }

    // the following string literal has a bare CR in it
    let _s = "foobar"; // { dg-error "" "" { target *-*-* } }

    // the following string literal has a bare CR in it
    let _s = r"barfoo"; // { dg-error "" "" { target *-*-* } }

    // the following string literal has a bare CR in it
    let _s = "foo\bar"; // { dg-error "" "" { target *-*-* } }
}

