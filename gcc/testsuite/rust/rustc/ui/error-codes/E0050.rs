trait Foo {
    fn foo(&self, x: u8) -> bool;
    fn bar(&self, x: u8, y: u8, z: u8);
    fn less(&self);
}

struct Bar;

impl Foo for Bar {
    fn foo(&self) -> bool { true } // { dg-error ".E0050." "" { target *-*-* } }
    fn bar(&self) { } // { dg-error ".E0050." "" { target *-*-* } }
    fn less(&self, x: u8, y: u8, z: u8) { } // { dg-error ".E0050." "" { target *-*-* } }
}

fn main() {
}

