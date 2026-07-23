fn main() {
    compile_error!("a very descriptive error message"); // { dg-error "" "" { target *-*-* } }
}

