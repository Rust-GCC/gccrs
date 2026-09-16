struct BuildData {
    foo: isize,
}

fn main() {
    let foo = BuildData {
        foo: 0,
        bar: 0
// { dg-error ".E0560." "" { target *-*-* } .-1 }
    };
}

