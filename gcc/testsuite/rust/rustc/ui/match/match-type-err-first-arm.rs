fn main() {
    let _ = test_func1(1);
    let _ = test_func2(1);
}

fn test_func1(n: i32) -> i32 { // { dg-note "" "" { target *-*-* } }
    match n {
        12 => 'b',
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
        _ => 42,
    }
}

fn test_func2(n: i32) -> i32 {
    let x = match n { // { dg-note ".E0308." "" { target *-*-* } }
        12 => 'b', // { dg-note "" "" { target *-*-* } }
        _ => 42,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
    };
    x
}

fn test_func3(n: i32) -> i32 {
    let x = match n { // { dg-note ".E0308." "" { target *-*-* } }
        1 => 'b',
        2 => 'b',
        3 => 'b',
        4 => 'b',
        5 => 'b',
        6 => 'b',
// { dg-note "" "" { target *-*-* } .-1 }
        _ => 42,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
    };
    x
}

fn test_func4() {
    match Some(0u32) { // { dg-note ".E0308." "" { target *-*-* } }
        Some(x) => {
            x // { dg-note "" "" { target *-*-* } }
        },
        None => {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
    };
}

