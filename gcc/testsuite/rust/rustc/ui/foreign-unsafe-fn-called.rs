mod test {
    extern {
        pub fn free();
    }
}

fn main() {
    test::free();
// { dg-error ".E0133." "" { target *-*-* } .-1 }
}

