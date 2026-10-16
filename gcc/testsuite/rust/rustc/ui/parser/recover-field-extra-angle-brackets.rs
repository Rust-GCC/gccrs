// Tests that we recover from extra trailing angle brackets
// in a struct field

struct BadStruct {
    first: Vec<u8>>, // { dg-error "" "" { target *-*-* } }
    second: bool
}

fn bar(val: BadStruct) {
    val.first;
    val.second;
}

fn main() {}

