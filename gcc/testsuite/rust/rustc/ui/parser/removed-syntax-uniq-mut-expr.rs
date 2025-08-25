fn main() {
    let a_box = box mut 42; // { dg-error "" "" { target *-*-* } }
}

