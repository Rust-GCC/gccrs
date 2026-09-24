macro_rules! test_macro {
    ( $( $t:ty ),* $(),*) => {
        enum SomeEnum {
            $( $t, )* // { dg-error "" "" { target *-*-* } }
        };
    };
}

fn main() {
    test_macro!(String,);
}

