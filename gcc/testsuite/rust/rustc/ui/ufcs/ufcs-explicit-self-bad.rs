#![feature(box_syntax)]

struct Foo {
    f: isize,
}

impl Foo {
    fn foo(self: isize, x: isize) -> isize {
// { dg-error ".E0307." "" { target *-*-* } .-1 }
        self.f + x
    }
}

struct Bar<T> {
    f: T,
}

impl<T> Bar<T> {
    fn foo(self: Bar<isize>, x: isize) -> isize {
// { dg-error ".E0307." "" { target *-*-* } .-1 }
        x
    }
    fn bar(self: &Bar<usize>, x: isize) -> isize {
// { dg-error ".E0307." "" { target *-*-* } .-1 }
        x
    }
}

trait SomeTrait {
    fn dummy1(&self);
    fn dummy2(&self);
    fn dummy3(&self);
}

impl<'a, T> SomeTrait for &'a Bar<T> {
    fn dummy1(self: &&'a Bar<T>) { }
    fn dummy2(self: &Bar<T>) {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    fn dummy3(self: &&Bar<T>) {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
// { dg-error ".E0308." "" { target *-*-* } .-7 }
// { dg-error ".E0308." "" { target *-*-* } .-8 }
}

fn main() {
    let foo = box Foo {
        f: 1,
    };
    println!("{}", foo.foo(2));
    let bar = box Bar {
        f: 1,
    };
    println!("{} {}", bar.foo(2), bar.bar(2));
}

