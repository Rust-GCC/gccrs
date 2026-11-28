mod foo {
    pub use self::bar::X;
    use self::bar::X;
// { dg-error ".E0252." "" { target *-*-* } .-1 }

    mod bar {
        pub struct X;
    }
}

fn main() {
    let _ = foo::X;
}

