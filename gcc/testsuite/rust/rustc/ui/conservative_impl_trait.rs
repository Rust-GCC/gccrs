// #39872, #39553

fn will_ice(something: &u32) -> impl Iterator<Item = &u32> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

