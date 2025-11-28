impl Foo for
fn main() {// { dg-error "unexpected token ..*. - expected bare function" }
// { dg-error "could not parse type in trait impl" "" { target *-*-* } .-1 }
}