// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

#[recollect_attr]
fn another() {
    fn bar() {
        let x: u32 = "x"; // { dg-error ".E0308." "" { target *-*-* } }
    }

    bar();
}

fn main() {
    #[recollect_attr]
    fn bar() {
        let x: u32 = "x"; // { dg-error ".E0308." "" { target *-*-* } }
    }

    bar();
    another();
}

