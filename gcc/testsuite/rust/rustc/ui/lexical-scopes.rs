struct T { i: i32 }
fn f<T>() {
    let t = T { i: 0 }; // { dg-error ".E0574." "" { target *-*-* } }
}

mod Foo {
    pub fn f() {}
}
fn g<Foo>() {
    Foo::f(); // { dg-error ".E0599." "" { target *-*-* } }
}

fn main() {}

