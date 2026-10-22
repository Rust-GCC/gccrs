#[derive(PartialEq)]
struct Thing(usize);
impl PartialOrd for Thing { // { dg-error ".E0046." "" { target *-*-* } }
    fn le(&self, other: &Thing) -> bool { true }
    fn ge(&self, other: &Thing) -> bool { true }
}
fn main() {}

