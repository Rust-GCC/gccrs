pub extern { // { dg-error ".E0449." "" { target *-*-* } }
    pub fn bar();
}

trait A {
    fn foo(&self) {}
}

struct B;

pub impl B {} // { dg-error ".E0449." "" { target *-*-* } }

pub impl A for B { // { dg-error ".E0449." "" { target *-*-* } }
    pub fn foo(&self) {} // { dg-error ".E0449." "" { target *-*-* } }
}

pub fn main() {}

