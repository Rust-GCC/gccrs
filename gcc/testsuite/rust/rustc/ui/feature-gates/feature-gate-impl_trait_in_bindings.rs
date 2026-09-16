const FOO: impl Copy = 42;
// { dg-error ".E0562." "" { target *-*-* } .-1 }

static BAR: impl Copy = 42;
// { dg-error ".E0562." "" { target *-*-* } .-1 }

fn main() {
    let foo = impl Copy = 42;
// { dg-error "" "" { target *-*-* } .-1 }
    let foo: impl Copy = 42;
}

