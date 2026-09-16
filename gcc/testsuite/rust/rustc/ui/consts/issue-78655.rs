const FOO: *const u32 = { // { dg-error "" "" { target *-*-* } }
    let x;
    &x // { dg-error ".E0381." "" { target *-*-* } }
};

fn main() {
    let FOO = FOO;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

