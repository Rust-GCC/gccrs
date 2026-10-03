// build-fail

fn main() {
    [1][0u64 as usize];
    [1][1.5 as usize]; // { dg-error "" "" { target *-*-* } }
    [1][1u64 as usize]; // { dg-error "" "" { target *-*-* } }
}

