struct BuildData {
    foo: isize,
}

fn main() {
    let foo = BuildData {
        foo: 0,
        foo: 0 // { dg-error ".E0062." "" { target *-*-* } }
    };
}

