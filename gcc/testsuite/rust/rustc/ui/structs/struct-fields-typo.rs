struct BuildData {
    foo: isize,
    bar: f32
}

fn main() {
    let foo = BuildData {
        foo: 0,
        bar: 0.5,
    };
    let x = foo.baa; // { dg-error ".E0609." "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
    println!("{}", x);
}

