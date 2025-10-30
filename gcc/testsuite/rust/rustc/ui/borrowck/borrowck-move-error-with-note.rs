#![feature(box_syntax)]

enum Foo {
    Foo1(Box<u32>, Box<u32>),
    Foo2(Box<u32>),
    Foo3,
}

fn blah() {
    let f = &Foo::Foo1(box 1, box 2);
    match *f {             // { dg-error ".E0507." "" { target *-*-* } }
        Foo::Foo1(num1,
                  num2) => (),
        Foo::Foo2(num) => (),
        Foo::Foo3 => ()
    }
}

struct S {
    f: String,
    g: String
}
impl Drop for S {
    fn drop(&mut self) { println!("{}", self.f); }
}

fn move_in_match() {
    match (S {f: "foo".to_string(), g: "bar".to_string()}) {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
        S {
            f: _s,
            g: _t
        } => {}
    }
}

// from issue-8064
struct A {
    a: Box<isize>,
}

fn free<T>(_: T) {}

fn blah2() {
    let a = &A { a: box 1 };
    match a.a {           // { dg-error ".E0507." "" { target *-*-* } }
        n => {
            free(n)
        }
    }
    free(a)
}

fn main() {}

