// ignore-tidy-linelength

struct SingleFoo {
    x: i32
}

struct PluralFoo {
    x: i32,
    y: i32,
    z: i32
}

struct TruncatedFoo {
    a: i32,
    b: i32,
    x: i32,
    y: i32,
    z: i32
}

struct TruncatedPluralFoo {
    a: i32,
    b: i32,
    c: i32,
    x: i32,
    y: i32,
    z: i32
}


fn main() {
    let w = SingleFoo { };
// { dg-error ".E0063." "" { target *-*-* } .-1 }
    let x = PluralFoo {x: 1};
// { dg-error ".E0063." "" { target *-*-* } .-1 }
    let y = TruncatedFoo{x:1};
// { dg-error ".E0063." "" { target *-*-* } .-1 }
    let z = TruncatedPluralFoo{x:1};
// { dg-error ".E0063." "" { target *-*-* } .-1 }
}

