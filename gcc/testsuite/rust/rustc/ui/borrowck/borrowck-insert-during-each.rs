use std::collections::HashSet;

struct Foo {
  n: HashSet<isize>,
}

impl Foo {
    pub fn foo<F>(&mut self, mut fun: F) where F: FnMut(&isize) {
        for f in &self.n {
            fun(f);
        }
    }
}

fn bar(f: &mut Foo) {
    f.foo(
// { dg-error ".E0501." "" { target *-*-* } .-1 }
        |a| { // { dg-error ".E0500." "" { target *-*-* } }
            f.n.insert(*a);
        })
}

fn main() {
  let mut f = Foo { n: HashSet::new() };
  bar(&mut f);
}

