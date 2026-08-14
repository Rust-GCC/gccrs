use bar::foo;

fn foo() {} // { dg-error ".E0255." "" { target *-*-* } }

mod bar {
     pub fn foo() {}
}

fn main() {}

