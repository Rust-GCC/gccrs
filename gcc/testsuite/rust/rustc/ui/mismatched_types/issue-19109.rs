trait Trait { }

fn function(t: &mut dyn Trait) {
    t as *mut dyn Trait
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() { }

