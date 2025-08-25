mod moon {
    pub fn foo() {}
}

mod earth {
    pub fn foo() {}
}

mod collider {
    pub use moon::*;
    pub use earth::*;
}

fn main() {
    collider::foo(); // { dg-error ".E0659." "" { target *-*-* } }
}

