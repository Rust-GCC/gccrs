enum FooEnum {
    #[test]
// { dg-error "" "" { target *-*-* } .-1 }
    Bar(i32),
}

struct FooStruct {
    #[test]
// { dg-error "" "" { target *-*-* } .-1 }
    bar: i32,
}

fn main() {
    let foo_enum_bar = FooEnum::Bar(1);
    match foo_enum_bar {
        FooEnum::Bar(x) => {},
        _ => {}
    }

    let foo_struct = FooStruct { bar: 1 };
    match foo_struct {
        FooStruct {
            #[test] bar
// { dg-error "" "" { target *-*-* } .-1 }
        } => {}
    }

    match 1 {
        0 => {}
        #[test]
// { dg-error "" "" { target *-*-* } .-1 }
        _ => {}
    }

    let _another_foo_strunct = FooStruct {
        #[test]
// { dg-error "" "" { target *-*-* } .-1 }
        bar: 1,
    };
}

