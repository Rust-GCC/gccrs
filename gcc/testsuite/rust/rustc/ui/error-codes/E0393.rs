trait A<T=Self> {}

fn together_we_will_rule_the_galaxy(son: &dyn A) {}
// { dg-error ".E0393." "" { target *-*-* } .-1 }

fn main() {
}

