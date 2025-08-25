fn main() {
    (0..13).collect<Vec<i32>>();
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    Vec<i32>::new();
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    (0..13).collect<Vec<i32>();
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    let x = std::collections::HashMap<i128, i128>::new(); // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    let x: () = 42; // { dg-error ".E0308." "" { target *-*-* } }
    let x = {
        std::collections::HashMap<i128, i128>::new() // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    };
    let x: () = 42; // { dg-error ".E0308." "" { target *-*-* } }
    let x = {
        std::collections::HashMap<i128, i128>::new(); // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
        let x: () = 42; // { dg-error ".E0308." "" { target *-*-* } }
    };
    {
        std::collections::HashMap<i128, i128>::new(1, 2); // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
        let x: () = 32; // { dg-error ".E0308." "" { target *-*-* } }
    };
}

