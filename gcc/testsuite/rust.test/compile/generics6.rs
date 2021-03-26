struct Foo<T>(T);

struct Bar<T> {
    a: Foo<T>,
    b: bool,
}

fn main() {
    let a: Bar<i32> = Bar::<i32> {
        a: Foo::<i32>(123),
        b: true,
    };
    let b: i32 = a.a.0;
}
