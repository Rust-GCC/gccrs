fn main() {
    let s: &[bool] = &[true; 0];
    let s1: &[bool; 1] = &[false; 1];
    let s2: &[bool; 2] = &[false; 2];
    let s3: &[bool; 3] = &[false; 3];
    let s10: &[bool; 10] = &[false; 10];

    match s2 {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [true, .., true] => {}
    }
    match s3 {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [true, .., true] => {}
    }
    match s10 {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [true, .., true] => {}
    }

    match s1 {
        [true, ..] => {}
        [.., false] => {}
    }
    match s2 {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [true, ..] => {}
        [.., false] => {}
    }
    match s3 {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [true, ..] => {}
        [.., false] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
        [true, ..] => {}
        [.., false] => {}
    }

    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
        [_] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
        [true, ..] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
        [_] => {}
        [true, ..] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
        [_] => {}
        [.., true] => {}
    }

    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
        [_] => {}
        [_, _] => {}
        [.., false] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        [] => {}
        [_] => {}
        [_, _] => {}
        [false, .., false] => {}
    }

    const CONST: &[bool] = &[true];
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        &[true] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        CONST => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        CONST => {}
        &[false] => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        &[false] => {}
        CONST => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        &[] => {}
        CONST => {}
    }
    match s {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        &[] => {}
        CONST => {}
        &[_, _, ..] => {}
    }
    match s {
        [] => {}
        [false] => {}
        CONST => {}
        [_, _, ..] => {}
    }
    const CONST1: &[bool; 1] = &[true];
    match s1 {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        CONST1 => {}
    }
    match s1 {
        CONST1 => {}
        [false] => {}
    }
}

