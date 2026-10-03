// build-fail

fn main() {
    let array = [std::env::args().len()];
    array[1]; // { dg-error "" "" { target *-*-* } }
}

