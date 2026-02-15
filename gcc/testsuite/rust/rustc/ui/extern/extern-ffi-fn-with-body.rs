extern "C" {
    fn foo() -> i32 { // { dg-error "" "" { target *-*-* } }
        return 0;
    }
}

extern "C" fn bar() -> i32 {
    return 0;
}

fn main() {}

