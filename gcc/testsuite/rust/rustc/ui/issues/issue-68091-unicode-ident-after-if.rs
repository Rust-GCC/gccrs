macro_rules! x {
    ($($c:tt)*) => {
        $($c)ö* {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() {
    x!(if);
}

