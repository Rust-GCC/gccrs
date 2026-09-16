// Used to cause ICE

static VEC: [u32; 256] = vec![];
// { dg-error ".E0308." "" { target *-*-* } .-1 }

fn main() {}

