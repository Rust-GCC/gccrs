fn main() {
    let _ = if true {
        1i32
    } else {
        2u32
    };
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    let _ = if true { 42i32 } else { 42u32 };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let _ = if true {
        3u32;
    } else {
        4u32
    };
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    let _ = if true {
        5u32
    } else {
        6u32;
    };
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    let _ = if true {
        7i32;
    } else {
        8u32
    };
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    let _ = if true {
        9i32
    } else {
        10u32;
    };
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    let _ = if true {

    } else {
        11u32
    };
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    let _ = if true {
        12i32
    } else {

    };
// { dg-error ".E0308." "" { target *-*-* } .-3 }
}

