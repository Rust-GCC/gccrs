struct MyStruct {
    pub s1: Option<String>,
}

fn main() {
    let thing = MyStruct { s1: None };

    match thing {
        MyStruct { .., Some(_) } => {},
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        _ => {}
    }
}

