impl Foo for
fn main() {// { dg-error "unexpected token .identifier. - expected bare function" }
// { dg-error "could not parse type in trait impl" "" { target *-*-* } .-1 }
}// { dg-error "failed to parse item in crate" "" { target *-*-* } .+1 }
