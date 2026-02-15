//extern crate has_assoc_type;

//fn ice(x: Box<dyn has_assoc_type::Foo<Assoc=()>>) {
fn ice(x: Box<dyn Iterator<Item=()>>) {
    *x // { dg-error ".E0308." "" { target *-*-* } }
}
fn main() {}

