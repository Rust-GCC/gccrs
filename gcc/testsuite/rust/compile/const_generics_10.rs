#![feature(lang_items)]
#[lang = "sized"]
trait Sized {}

const M: usize = 4;

struct Foo<T, const N: usize = 1> {
    value: [T; N],
}

fn main() {
    let foo = Foo::<i32> { value: [15] };
    let foo = Foo::<i32, 2> { value: [15, 13] };
    let foo: Foo<i32, 2> = Foo { value: [15, 13] };
    let foo: Foo<i32, 2> = Foo::<i32, 2> { value: [15, 13] };
    let foo: Foo<i32, { 1 + 1 }> = Foo { value: [15, 13] };
    let foo = Foo::<i32, { 1 + 1 }> { value: [15, 13] };
    let foo: Foo<i32, { 1 + 1 }> = Foo::<i32, { 1 + 1 }> { value: [15, 13] };
    let foo: Foo<i32, M> = Foo::<i32, 4> {
        value: [15, 13, 11, 9],
    };

    let invalid_foo: Foo<i32, { 1 + 1 }> = Foo::<i32, 3> { value: [15, 13] };
    // { dg-error "mismatched types" "" { target *-*-* } .-1 }

    let invalid_foo: Foo<i32, { 1 + 1 }> = Foo::<i32, M> { value: [15, 13] };
    // { dg-error "mismatched types" "" { target *-*-* } .-1 }

    let invalid_foo: Foo<i32, { 1 + 2 }> = Foo::<i32, 2> { value: [15, 13] };
    // { dg-error "mismatched types" "" { target *-*-* } .-1 }
}
