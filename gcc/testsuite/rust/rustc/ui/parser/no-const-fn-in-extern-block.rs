extern {
    const fn foo();
// { dg-error "" "" { target *-*-* } .-1 }
    const unsafe fn bar();
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

