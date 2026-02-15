fn foo() -> impl std::fmt::Display {
    if false {
        return 0i32;
    }
    1u32 // { dg-error ".E0308." "" { target *-*-* } }
}

fn bar() -> impl std::fmt::Display {
    if false {
        return 0i32;
    } else {
        return 1u32; // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn baz() -> impl std::fmt::Display {
    if false {
        return 0i32;
    } else {
        1u32 // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn qux() -> impl std::fmt::Display {
    if false {
        0i32
    } else {
        1u32 // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn bat() -> impl std::fmt::Display {
    match 13 {
        0 => return 0i32,
        _ => 1u32, // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn can() -> impl std::fmt::Display {
    match 13 { // { dg-error ".E0308." "" { target *-*-* } }
        0 => return 0i32,
        1 => 1u32,
        _ => 2u32,
    }
}

fn cat() -> impl std::fmt::Display {
    match 13 {
        0 => {
            return 0i32;
        }
        _ => {
            1u32 // { dg-error ".E0308." "" { target *-*-* } }
        }
    }
}

fn dog() -> impl std::fmt::Display {
    match 13 {
        0 => 0i32,
        1 => 1u32, // { dg-error ".E0308." "" { target *-*-* } }
        _ => 2u32,
    }
}

fn hat() -> dyn std::fmt::Display { // { dg-error ".E0746." "" { target *-*-* } }
    match 13 {
        0 => {
            return 0i32;
        }
        _ => {
            1u32
        }
    }
}

fn pug() -> dyn std::fmt::Display { // { dg-error ".E0746." "" { target *-*-* } }
    match 13 {
        0 => 0i32,
        1 => 1u32, // { dg-error ".E0308." "" { target *-*-* } }
        _ => 2u32,
    }
}

fn man() -> dyn std::fmt::Display { // { dg-error ".E0746." "" { target *-*-* } }
    if false {
        0i32
    } else {
        1u32 // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn apt() -> impl std::fmt::Display {
    if let Some(42) = Some(42) {
        0i32
    } else {
        1u32 // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn main() {}

