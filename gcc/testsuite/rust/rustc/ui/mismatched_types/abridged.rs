enum Bar {
    Qux,
    Zar,
}

struct Foo {
    bar: usize,
}

struct X<T1, T2> {
    x: T1,
    y: T2,
}

fn a() -> Foo {
    Some(Foo { bar: 1 }) // { dg-error ".E0308." "" { target *-*-* } }
}

fn a2() -> Foo {
    Ok(Foo { bar: 1}) // { dg-error ".E0308." "" { target *-*-* } }
}

fn b() -> Option<Foo> {
    Foo { bar: 1 } // { dg-error ".E0308." "" { target *-*-* } }
}

fn c() -> Result<Foo, Bar> {
    Foo { bar: 1 } // { dg-error ".E0308." "" { target *-*-* } }
}

fn d() -> X<X<String, String>, String> {
    let x = X {
        x: X {
            x: "".to_string(),
            y: 2,
        },
        y: 3,
    };
    x // { dg-error ".E0308." "" { target *-*-* } }
}

fn e() -> X<X<String, String>, String> {
    let x = X {
        x: X {
            x: "".to_string(),
            y: 2,
        },
        y: "".to_string(),
    };
    x // { dg-error ".E0308." "" { target *-*-* } }
}

fn f() -> String {
    1+2 // { dg-error ".E0308." "" { target *-*-* } }
}


fn g() -> String {
    -2 // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

