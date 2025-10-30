#![feature(stmt_expr_attributes)]

fn main() {

    #[inline]
    let _a = 4;
// { dg-error ".E0518." "" { target *-*-* } .-2 }


    #[inline(XYZ)]
    let _b = 4;
// { dg-error ".E0518." "" { target *-*-* } .-2 }

    #[repr(nothing)]
    let _x = 0;
// { dg-error ".E0517." "" { target *-*-* } .-2 }

    #[repr(something_not_real)]
    loop {
        ()
    };
// { dg-error ".E0517." "" { target *-*-* } .-4 }

    #[repr]
    let _y = "123";
// { dg-error "" "" { target *-*-* } .-2 }

    fn foo() {}

    #[inline(ABC)]
    foo();
// { dg-error ".E0518." "" { target *-*-* } .-2 }

    let _z = #[repr] 1;
// { dg-error "" "" { target *-*-* } .-1 }
}

