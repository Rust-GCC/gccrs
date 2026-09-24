use foo::bar::{ // { dg-error ".E0603." "" { target *-*-* } }
    self
};
use foo::bar::{ // { dg-error ".E0603." "" { target *-*-* } }
    Bar
};

mod foo {
    mod bar { pub type Bar = isize; }
}

fn main() {}

