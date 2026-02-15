fn main() { env!("__HOPEFULLY_NOT_DEFINED__", "my error message"); } // { dg-error "" "" { target *-*-* } }

