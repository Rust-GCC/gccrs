const FOO: *const u32 = { // { dg-error "" "" { target *-*-* } }
    let x = 42;
    &x
};

fn main() {
    let x = FOO;
}

