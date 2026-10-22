use std::ops::{Index, IndexMut};

struct Foo {
    x: isize,
    y: isize,
}

impl Index<String> for Foo {
    type Output = isize;

    fn index(&self, z: String) -> &isize {
        if z == "x" {
            &self.x
        } else {
            &self.y
        }
    }
}

impl IndexMut<String> for Foo {
    fn index_mut(&mut self, z: String) -> &mut isize {
        if z == "x" {
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

    println!("{}", f[s]);
// { dg-error ".E0505." "" { target *-*-* } .-1 }

    f[s] = 10;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }

    let s = Bar {
        x: 1,
    };
    let i = 2;
    let _j = &i;
    println!("{}", s[i]); // no error, i is copy
    println!("{}", s[i]);

    use_mut(rs);
}

fn use_mut<T>(_: &mut T) { }

