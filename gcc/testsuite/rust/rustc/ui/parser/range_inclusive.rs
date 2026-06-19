// run-rustfix
// Make sure that inclusive ranges with no end point don't parse.

pub fn main() {
    for _ in 1..= {} // { dg-error ".E0586." "" { target *-*-* } }
// { help ".E0586." "" { target *-*-* } .-1 }
}

