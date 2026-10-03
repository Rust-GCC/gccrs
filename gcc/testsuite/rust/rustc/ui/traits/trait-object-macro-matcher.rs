// `ty` matcher accepts trait object types

macro_rules! m {
    ($t: ty) => ( let _: $t; )
}

fn main() {
    m!(dyn Copy + Send + 'static);
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    m!(dyn 'static + Send);
    m!(dyn 'static +); // { dg-error ".E0224." "" { target *-*-* } }
}

