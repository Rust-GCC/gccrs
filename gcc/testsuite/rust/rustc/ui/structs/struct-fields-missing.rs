struct BuildData {
    foo: isize,
    bar: Box<isize>,
}

fn main() {
    let foo = BuildData { // { dg-error ".E0063." "" { target *-*-* } }
        foo: 0
    };
}

