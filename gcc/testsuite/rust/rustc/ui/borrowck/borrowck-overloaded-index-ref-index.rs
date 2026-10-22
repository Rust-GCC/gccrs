use std::ops::{Index, IndexMut};

struct Foo {
    x: isize,
    y: isize,
}

impl<'a> Index<&'a String> for Foo {
    type Output = isize;

    fn index(&self, z: &String) -> &isize {
        if *z == "x" {
            &self.x
        } else {
            &self.y
        }
    }
}

impl<'a> IndexMut<&'a String> for Foo {
    fn index_mut(&mut self, z: &String) -> &mut isize {
        if *z == "x" {
            &mut self.x
        } else {
            &mut self.y
        }
    }
}

struct Bar {
    x: isize,
}

impl Index<isize> for Bar {
    type Output = isize;

    fn index<'a>(&'a self, z: isize) -> &'a isize {
        &self.x
    }
}

fn main() {
    let mut f = Foo {
        x: 1,
        y: 2,
    };
    let mut s = "hello".to_string();
    let rs = &mut s;
    println!("{}", f[&s]);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    f[&s] = 10;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    let s = Bar {
        x: 1,
    };
    s[2] = 20;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
    drop(rs);
}

