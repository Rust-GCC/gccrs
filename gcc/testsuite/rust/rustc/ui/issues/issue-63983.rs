enum MyEnum {
    Tuple(i32),
    Struct{ s: i32 },
}

fn foo(en: MyEnum) {
    match en {
        MyEnum::Tuple => "",
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        MyEnum::Struct => "",
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    };
}

fn main() {}

