trait Deserializer<'a> { }

trait Deserializable {
    fn deserialize_token<'a, D: Deserializer<'a>>(_: D, _: &'a str) -> Self;
}

impl<'a, T: Deserializable> Deserializable for &'a str {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
    fn deserialize_token<D: Deserializer<'a>>(_x: D, _y: &'a str) -> &'a str {
    }
}

fn main() {}

