struct X(());
impl X {
    pub unsafe fn with(&self) { }
}

fn main() {
    X(()).with(); // { dg-error ".E0133." "" { target *-*-* } }
}

