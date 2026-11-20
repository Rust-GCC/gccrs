macro_rules! mac {
    ( $($v:tt)* ) => {
        $v
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    };
}

fn main() {
    mac!(0);
    mac!(1);
}

