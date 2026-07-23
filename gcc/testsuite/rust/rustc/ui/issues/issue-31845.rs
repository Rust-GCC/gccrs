// Checks lexical scopes cannot see through normal module boundaries

fn f() {
    fn g() {}
    mod foo {
        fn h() {
           g(); // { dg-error ".E0425." "" { target *-*-* } }
        }
    }
}

fn main() {}

