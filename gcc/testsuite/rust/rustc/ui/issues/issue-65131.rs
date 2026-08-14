fn get_pair(_a: &mut u32, _b: &mut u32) {}

macro_rules! x10 {
    ($($t:tt)*) => {
        $($t)* $($t)* $($t)* $($t)* $($t)*
        $($t)* $($t)* $($t)* $($t)* $($t)*
    }
}

#[allow(unused_assignments)]
fn main() {
    let mut x = 1;

    get_pair(&mut x, &mut x);
// { dg-error ".E0499." "" { target *-*-* } .-1 }

    x10! { x10!{ x10!{ if x > 0 { x += 2 } else { x += 1 } } } }
}

