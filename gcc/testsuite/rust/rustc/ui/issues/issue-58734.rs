trait Trait {
    fn exists(self) -> ();

    fn not_object_safe() -> Self;
}

impl Trait for () {
    fn exists(self) -> () {
    }

    fn not_object_safe() -> Self {
        ()
    }
}

fn main() {
    // object-safe or not, this call is OK
    Trait::exists(());
    // no object safety error
    Trait::nonexistent(());
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

