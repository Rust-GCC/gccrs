#![feature(type_alias_impl_trait)]

fn main() {}

mod boo {
    pub type Boo = impl ::std::fmt::Debug;
    fn bomp() -> Boo {
        ""
    }
}

// We don't actually know the type here.

fn bomp2() {
    let _: &str = bomp(); // { dg-error ".E0308." "" { target *-*-* } }
}

fn bomp() -> boo::Boo {
    "" // { dg-error ".E0308." "" { target *-*-* } }
}

fn bomp_loop() -> boo::Boo {
    loop {}
}

