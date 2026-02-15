// build-fail

fn main() {
    [0; 3][3u64 as usize]; // { dg-error "" "" { target *-*-* } }
}

