// Regression test for the issue #44692

macro_rules! hang { () => {
    { // { dg-error "" "" { target *-*-* } }
        #[derive(Clone)]
        struct S;

        ""
    }
}}

fn main() {
    format_args!(hang!());
}

