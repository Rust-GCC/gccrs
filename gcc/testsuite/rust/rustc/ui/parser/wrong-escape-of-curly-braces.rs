fn main() {
    let ok = "{{everything fine}}";
    let bad = "\{it is wrong\}";
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
}

