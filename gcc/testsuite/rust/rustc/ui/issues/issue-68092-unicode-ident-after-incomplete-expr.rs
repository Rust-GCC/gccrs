macro_rules! x {
    ($($c:tt)*) => {
        $($c)ö* // { dg-error "" "" { target *-*-* } }
    };
}

fn main() {
    x!(!);
}

