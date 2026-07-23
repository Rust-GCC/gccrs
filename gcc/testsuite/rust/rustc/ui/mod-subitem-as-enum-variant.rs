mod Mod {
    pub struct FakeVariant<T>(pub T);
}

fn main() {
    Mod::FakeVariant::<i32>(0);
    Mod::<i32>::FakeVariant(0);
// { dg-error ".E0109." "" { target *-*-* } .-1 }
}

