macro_rules! get_opt {
    ($tgt:expr, $field:ident) => {
        if $tgt.has_$field() {} // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {
    get_opt!(bar, foo);
}

