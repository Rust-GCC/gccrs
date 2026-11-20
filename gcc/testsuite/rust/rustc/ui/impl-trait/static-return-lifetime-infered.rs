struct A {
    x: [(u32, u32); 10]
}

impl A {
    fn iter_values_anon(&self) -> impl Iterator<Item=u32> {
        self.x.iter().map(|a| a.0) // { dg-error ".E0759." "" { target *-*-* } }
    }
    fn iter_values<'a>(&'a self) -> impl Iterator<Item=u32> {
        self.x.iter().map(|a| a.0) // { dg-error ".E0759." "" { target *-*-* } }
    }
}

fn main() {}

