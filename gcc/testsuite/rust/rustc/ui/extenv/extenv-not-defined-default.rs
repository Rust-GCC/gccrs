fn main() {
    env!("__HOPEFULLY_NOT_DEFINED__");
// { dg-error "" "" { target *-*-* } .-1 }
}

