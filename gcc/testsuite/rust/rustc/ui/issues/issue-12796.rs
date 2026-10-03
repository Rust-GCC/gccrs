trait Trait {
    fn outer(&self) {
        fn inner(_: &Self) {
// { dg-error ".E0401." "" { target *-*-* } .-1 }
        }
    }
}

fn main() { }

