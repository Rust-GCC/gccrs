fn main() {
    let _ = Iterator::next(&mut ());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    for _ in false {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    let _ = Iterator::next(&mut ());
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    other()
}

pub fn other() {
    // check errors are still reported globally

    let _ = Iterator::next(&mut ());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    let _ = Iterator::next(&mut ());
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    for _ in false {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

