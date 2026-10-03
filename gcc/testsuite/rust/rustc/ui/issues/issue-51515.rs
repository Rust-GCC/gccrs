fn main() {
    let foo = &16;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    *foo = 32;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
    let bar = foo;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    *bar = 64;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
}

