fn test(x: i32) -> i32 {
    if x > 1 { // { dg-error "expected .... got .<integer>." }
        1
    } else {
        2
    }

    { // { dg-error "expected .... got .<integer>." }
        3
    }

    if x > 1 { 1 } else { 2 };
    if x > 1 { 1; } else { 2; }

    { 3; }
    { 3 };

    { 3 }
}

fn main() {
    let a = test(1);
}
