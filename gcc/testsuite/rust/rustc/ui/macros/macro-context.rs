// (typeof used because it's surprisingly hard to find an unparsed token after a stmt)
macro_rules! m {
    () => ( i ; typeof );   // { dg-error ".E0425." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error ".E0412." "" { target *-*-* } .-5 }
// { dg-error ".E0425." "" { target *-*-* } .-6 }
}

fn main() {
    let a: m!();
    let i = m!();
    match 0 {
        m!() => {}
    }

    m!();
}

